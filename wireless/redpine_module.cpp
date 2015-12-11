//
//  redpine_module.cpp
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//

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
}

// STATIC PUBLIC METHODS

Module* Module::Instance()
{
    return &Module::moduleSingleton;
}

bool Module::initialize(ModuleCommunication *commInterface)
{
    
    if (commInterface == NULL)
    {
        error("Cannot init Redpine Module without comm. interface!\n\r");
        return false;
    }
    
    // Assign interface to object
    Module* self = Module::Instance();
    self->comIntf = commInterface;
    
    self->comIntf->resetModule();
    
    self->CurrentPowerState = FULL_AWAKE;
    
    debug("Initializing communication interface...\n\r");
    // initalize interface
    bool success = self->comIntf->initializeInterface();
    
    if (!success)
    {
        error("Initialize failed to init communication interface\n\r");
        return false;
    }
    
    mono::IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(self);
    
    debug("Checking bootloader state...\n\r");
    uint16_t regval = self->comIntf->readMemory(HOST_INTF_REG_OUT);
    debug("HOST_INTF_REG_OUT: 0x%x\n\r", regval);
    
    if ((regval & HOST_INTERACT_REG_VALID) == 0xAB00)
    {
        debug("Module is in bootloader, load Image-I...\n\r");
        self->comIntf->InterfaceVersion = regval;
        self->comIntf->writeMemory(HOST_INTF_REG_IN, HOST_INTERACT_REG_VALID | RSI_LOAD_IMAGE_I_FW);
    }
    
    // we need to wait for board ready frame
    debug("Waiting for card ready to arrive on input...\n\r");
    int timeout = 0;
    while (!self->comIntf->pollInputQueue() && timeout++ < 50)
    {
        //wait a while
        wait_ms(2);
    }
    
    if (timeout >= 50)
    {
        debug("Timeout: Did not receive Card ready!\n\r");
        return false;
    }
    
    ManagementFrame frame;
    self->comIntf->readManagementFrame(frame);
    
    self->comIntf->interruptCallback.attach<mono::redpine::Module>(self, &Module::moduleEventHandler);
    
    if (frame.commandId == ModuleFrame::CardReady)
    {
        debug("Card ready received\n\r");
        return true;
    }
    else
    {
        debug("Initialization failed on receiving card ready\n\r");
        debug("Initialization got 0x%x, not card ready\n\r",frame.commandId);
        return false;
    }
    
}

bool Module::setupWifiOnly(String ssid, String passphrase, WifiSecurityModes secMode)
{
    // send "set operating mode command"
    
    //debug("Setting OperMode...\n\r");
    SetOperatingModeFrame *opermode = new SetOperatingModeFrame(SetOperatingModeFrame::WIFI_CLIENT_MODE);
    opermode->setDefaultConfiguration();
    opermode->commitAsync();
    
    //debug("Setting band...\n\r");
    BandFrame *band = new BandFrame();
    band->commitAsync();

    //debug("Initialize module RF and baseband...\n\r");
    InitFrame *init = new InitFrame();
    init->commitAsync();

    //debug("Scanning for networks...\n\r");
    ScanFrame *scan = new ScanFrame();
    scan->commitAsync();
    
    //debug("Connecting to %s\n\r",ssid);
    JoinFrame *join = new JoinFrame(ssid, passphrase, secMode);
    join->commitAsync();

    //debug("Getting IP address from DHCP...\n\r");
    SetIpParametersFrame *ipparam = new SetIpParametersFrame();
    ipparam->setCompletionCallback<Module>(Module::Instance(), &Module::onNetworkReady);
    
    ipparam->commitAsync();
    
    return true;
}

bool Module::IsNetworkReady()
{
    return Module::Instance()->networkInitialized;
}


//// PRIVATE METHODS

void Module::moduleEventHandler()
{
    
    if (CurrentPowerState & (LOW_SLEEP | ULTRA_LOW_SLEEP))
        handleSleepWakeUp();
    else
    {
        
        if (comIntf->pollInputQueue())
        {
            //handle a response for any pending request
            ManagementFrame *respFrame = responseFrameQueue.Peek();
            if (respFrame == NULL)
            {
                debug("nothing on request queue!\n\r");
                ManagementFrame frame;
                comIntf->readManagementFrame(frame);
            }
            
            debug("resp frame cmd id: 0x%x\n\r",respFrame->commandId);
            //memdump(reqFrame, sizeof(mono::redpine::HttpGetFrame));
            
            bool success = this->comIntf->readManagementFrameResponse(*respFrame);
            
            if (!success) {
                responseFrameQueue.Remove(respFrame);
                respFrame->triggerCompletionHandler();
                
                error("failed to handle incoming response for resp queue head\n\r");
            }
            else if (respFrame->lastResponseParsed)
            {
                debug("Frame (0x%x): last response parsed!\n\r",respFrame->commandId);
                responseFrameQueue.Remove(respFrame);
                respFrame->triggerCompletionHandler();
                
                if (respFrame->autoReleaseWhenParsed)
                {
                    debug("deleting resp frame 0x%x!\n\r",respFrame->commandId);
                    delete respFrame;
                }
                else
                {
                    debug("leaving frame 0x%x\n\r",respFrame->commandId);
                }
                
            }

        }
        
        // if no response is pending, send new request
        if (responseFrameQueue.Length() == 0 && requestFrameQueue.Length() > 0)
        {
            ManagementFrame *request = requestFrameQueue.Dequeue();
            debug("Sending Mgmt request 0x%x\n\r",request->commandId);
            bool success = request->writeFrame();
            
            if (!success)
            {
                debug("Failed to send MgmtFrame (0x%x) to module\n\r",request->commandId);
                
                if (request->autoReleaseWhenParsed)
                    delete request;
                
                return;
            }
            
            responseFrameQueue.Enqueue(request);
        }
        
    }
    
}

void Module::onNetworkReady(ManagementFrame::FrameCompletionData *data)
{
    if (data->Context->commandId == ModuleFrame::SetIPParameters)
    {
        SetIpParametersFrame *ip = (SetIpParametersFrame*) data->Context;
        
        memcpy(this->ipAddress, ip->ipAddress, 4);
        memcpy(this->netmask, ip->netmask, 4);
        memcpy(this->gateway, ip->gateway, 4);
        memcpy(this->macAddress, ip->macAddress, 6);
        
        debug("Network Ready!\n\r");
        networkInitialized = true;
        
        if (networkReadyHandler) {
            networkReadyHandler.call();
        }
    }
}

void Module::handleSleepWakeUp()
{
    
    if (!(CurrentPowerState & (ULTRA_LOW_SLEEP | LOW_SLEEP)))
    {
        debug("Module not in sleep!\n\r");
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
            debug("putting to sleep again!\n\r");
            ManagementFrame sleepAgain(ManagementFrame::PowerSaveACK);
            sleepAgain.commit();
            
        }
        else
        {
            debug("Unkown commandID for handleSleepWakeUp: 0x%x\n\r",frame.commandId);
        }
    }
    else
    {
        debug("nothing in input poll queue\n\r");
    }
}

void Module::onSystemPowerOnReset()
{
    
}

void Module::onSystemEnterSleep()
{
    comIntf->resetModule();
}

void Module::onSystemWakeFromSleep()
{
    
}

void Module::OnSystemBatteryLow()
{
    
}








