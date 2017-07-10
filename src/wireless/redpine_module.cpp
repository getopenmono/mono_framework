// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "redpine_module.h"
#include <consoles.h>
#include <mbed.h>

#include <application_context_interface.h>

using namespace mono::redpine;

/** Construct the module */
Module Module::moduleSingleton;

Module::Module()
{
    communicationInitialized = false;
    networkInitialized = false;
    joinFailed = false;
}

/// MARK: STATIC PUBLIC METHODS

Module* Module::Instance()
{
    return &Module::moduleSingleton;
}

bool Module::initialize(ModuleCommunication *commInterface)
{

    if (commInterface == NULL)
    {
        debug("Cannot init Redpine Module without comm. interface!\r\n");
        return false;
    }

    // Assign interface to object
    Module* self = Module::Instance();
    self->comIntf = commInterface;

    self->comIntf->resetModule();

    self->CurrentPowerState = FULL_AWAKE;

    //debug("Initializing communication interface...\r\n");
    // initalize interface
    bool success = self->comIntf->initializeInterface();

    if (!success)
    {
        debug("Initialize failed to init communication interface\r\n");
        return false;
    }

    mono::IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(self);

    //debug("Checking bootloader state...\r\n");
    uint16_t regval = self->comIntf->readMemory(HOST_INTF_REG_OUT);
    debug("HOST_INTF_REG_OUT: 0x%x\r\n", regval);

    if ((regval & HOST_INTERACT_REG_VALID) == 0xAB00)
    {
        //debug("Module is in bootloader, load Image-I...\r\n");
        self->comIntf->InterfaceVersion = regval;
        self->comIntf->writeMemory(HOST_INTF_REG_IN, HOST_INTERACT_REG_VALID | RSI_LOAD_IMAGE_I_FW);
    }

    // we need to wait for board ready frame
    //debug("Waiting for card ready to arrive on input...\r\n");
    int timeout = 0;
    while (!self->comIntf->pollInputQueue() && timeout++ < 50)
    {
        //wait a while
        wait_ms(2);
    }

    if (timeout >= 50)
    {
        debug("Timeout: Did not receive Card ready!\r\n");
        return false;
    }

    ManagementFrame frame;
    success = self->comIntf->readManagementFrame(frame);

    if (!success)
    {
        debug("failed to read card ready!");
        return false;
    }

    self->comIntf->interruptCallback.attach<mono::redpine::Module>(self, &Module::moduleEventHandler);

    if (frame.commandId == ModuleFrame::CardReady)
    {
        //debug("Card ready received\r\n");
        self->communicationInitialized = true;
        return true;
    }
    else
    {
        debug("Initialization failed on receiving card ready\r\n");
        debug("Initialization got 0x%x, not card ready\r\n",frame.commandId);
        return false;
    }

}

bool Module::setupWifiOnly(String ssid, String passphrase, WifiSecurityModes secMode)
{
    // send "set operating mode command"
    Module *self = Module::Instance();

    if (!self->communicationInitialized)
    {
        debug("Module not initialized. You must initialize first!");
        return false;
    }

    //debug("Setting OperMode...\r\n");
    SetOperatingModeFrame *opermode = new SetOperatingModeFrame(SetOperatingModeFrame::WIFI_CLIENT_MODE);
    opermode->setDefaultConfiguration();
    opermode->commitAsync();

    //debug("Setting band...\r\n");
    BandFrame *band = new BandFrame();
    band->commitAsync();

    //debug("Initialize module RF and baseband...\r\n");
    InitFrame *init = new InitFrame();
    init->commitAsync();

    //debug("Scanning for networks...\r\n");
    ScanFrame *scan = new ScanFrame();
    scan->commitAsync();

    //debug("Connecting to %s\r\n",ssid);
    JoinFrame *join = new JoinFrame(ssid, passphrase, secMode);
    join->commitAsync();
    join->setCompletionCallback<Module>(self, &Module::onJoinNetworkComplete);

    //debug("Getting IP address from DHCP...\r\n");
    SetIpParametersFrame *ipparam = new SetIpParametersFrame();
    if (self->staticIPHandler)
    {
        ipparam->dhcpMode = SetIpParametersFrame::STATIC_IP;
        StaticIPParams params;
        self->staticIPHandler.call(&params);
        memcpy(ipparam->ipAddress, params.ipAddress, 4);
        memcpy(ipparam->netmask, params.netmask, 4);
        memcpy(ipparam->gateway, params.gateway, 4);
    }

    ipparam->setCompletionCallback<Module>(Module::Instance(), &Module::onNetworkReady);

    ipparam->commitAsync();

    return true;
}

bool Module::IsNetworkReady()
{
    Module *self = Module::Instance();
    return self->networkInitialized && self->joinFailed == false;
}


/// MARK: PRIVATE METHODS

void Module::moduleEventHandler()
{

    if (CurrentPowerState & (LOW_SLEEP | ULTRA_LOW_SLEEP))
        handleSleepWakeUp();
    else
    {

        if (comIntf->pollInputQueue())
        {
            //handle a response for any pending request
            ManagementFrame *respFrame = responseFrameQueue.peek();
            if (respFrame == NULL)
            {
                debug("nothing on request queue!\r\n");
                ManagementFrame frame;
                bool success = comIntf->readManagementFrame(frame);
                if (!success)
                {
                    debug("Failed to read unexcepted mgmt frame\r\n");
                }
            }
            else
            {
                debug("resp frame cmd id: 0x%x\r\n",respFrame->commandId);
                //memdump(reqFrame, sizeof(mono::redpine::HttpGetFrame));

                bool success = this->comIntf->readManagementFrameResponse(*respFrame);

                if (!success) {
                    responseFrameQueue.remove(respFrame);
                    debug("failed to handle incoming response for resp queue head\r\n");
                    respFrame->status = 1;
                    respFrame->triggerCompletionHandler();
                }
                else if (respFrame->lastResponseParsed)
                {
                    //debug("Frame (0x%x): last response parsed!\r\n",respFrame->commandId);
                    responseFrameQueue.remove(respFrame);
                    respFrame->triggerCompletionHandler();

                    if (respFrame->autoReleaseWhenParsed)
                    {
                        //debug("deleting resp frame 0x%x!\r\n",respFrame->commandId);
                        delete respFrame;
                    }
                    else
                    {
                        debug("leaving frame 0x%x\r\n",respFrame->commandId);
                    }

                }

            }

        }

        // if no response is pending, send new request
        if (responseFrameQueue.Length() == 0 && requestFrameQueue.Length() > 0)
        {
            ManagementFrame *request = requestFrameQueue.dequeue();
            //debug("Sending Mgmt request 0x%x\r\n",request->commandId);
            bool success = request->writeFrame();

            if (!success)
            {
                debug("Failed to send MgmtFrame (0x%x) to module\r\n",request->commandId);
                request->status = 1;
                request->triggerCompletionHandler();

                if (request->autoReleaseWhenParsed)
                    delete request;

                return;
            }

            responseFrameQueue.enqueue(request);
        }
    }
}

void Module::onNetworkReady(ManagementFrame::FrameCompletionData *data)
{
    if (data->Context->commandId == ModuleFrame::SetIPParameters
        && joinFailed == false && data->Success)
    {
        SetIpParametersFrame *ip = (SetIpParametersFrame*) data->Context;

        memcpy(this->ipAddress, ip->ipAddress, 4);
        memcpy(this->netmask, ip->netmask, 4);
        memcpy(this->gateway, ip->gateway, 4);
        memcpy(this->macAddress, ip->macAddress, 6);

        debug("Network Ready!\r\n");
        networkInitialized = true;

        if (networkReadyHandler) {
            networkReadyHandler.call();
        }
    }
    else
        connectFailedHandler.call();
}

void Module::onJoinNetworkComplete(ManagementFrame::FrameCompletionData *data)
{
    if (data->Context->commandId == ModuleFrame::Join)
    {
        if (!data->Success)
        {
            joinFailed = true;
            debug("Failed joining Network!\r\n");
            connectFailedHandler.call();
        }
        else {
            debug("Joined Wifi Network!\r\n");
        }
    }
}

void Module::handleSleepWakeUp()
{

    if (!(CurrentPowerState & (ULTRA_LOW_SLEEP | LOW_SLEEP)))
    {
        debug("Module not in sleep!\r\n");
        return;
    }

    // if interface was initialized, we are out of sleep
    if (comIntf->initializeInterface())
        CurrentPowerState = FULL_AWAKE;

    if (comIntf->pollInputQueue())
    {
        ManagementFrame frame;
        comIntf->readManagementFrame(frame);

        if (frame.commandId == ManagementFrame::WakeFromSleep)
        {
            debug("putting to sleep again!\r\n");
            ManagementFrame sleepAgain(ManagementFrame::PowerSaveACK);
            sleepAgain.commit();

        }
        else
        {
            debug("Unkown commandID for handleSleepWakeUp: 0x%x\r\n",frame.commandId);
        }
    }
    else
    {
        debug("nothing in input poll queue\r\n");
    }
}

void Module::onSystemPowerOnReset()
{

}

void Module::onSystemEnterSleep()
{
    //comIntf->resetModule();
}

void Module::onSystemWakeFromSleep()
{

}

void Module::onSystemBatteryLow()
{

}








