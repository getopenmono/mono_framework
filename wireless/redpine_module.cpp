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
        return false;
    
    // Assign interface to object
    Module* self = Module::Instance();
    self->comIntf = commInterface;
    
    mono::Debug << "Initializing communication interface...\n";
    // initalize interface
    bool success = self->comIntf->initializeInterface();
    
    if (!success)
    {
        mono::Error << "Initialize failed to init communication interface\n";
        return false;
    }
    
    // we need to wait for board ready frame
    mono::Debug << "Waiting for card ready to arrive on input...\n";
    while (!self->comIntf->pollInputQueue())
    {
        //wait a while
        mbed::wait_ms(200);
    }
    
    mono::Debug << "Input pending, will read frame...\n";
    ManagementFrame frame;
    self->comIntf->readManagementFrame(frame);
    
    if (frame.commandId == ModuleFrame::CardReady)
    {
        mono::Debug << "Card ready received" << "\n";
        return true;
    }
    else
    {
        mono::Error << "Initialization failed on receiving card ready\n";
        mono::Debug << "Initialization got " << frame.commandId << " not card ready\n";
        return false;
    }
    
}

bool Module::setupWifiOnly(const char *ssid, const char *passphrase, WifiSecurityModes secMode)
{
    // send "set operating mode command"
    
    mono::Debug << "Setting OperMode...\n";
    SetOperatingModeFrame opermode(SetOperatingModeFrame::WIFI_CLIENT_MODE);
    opermode.setDefaultConfiguration();
    opermode.commit();
    
    mono::Debug << "Setting band...\n";
    BandFrame band;
    band.commit();
    
    mono::Debug << "Initialize module RF and baseband...\n";
    InitFrame init;
    init.commit();
    
    mono::Debug << "Scanning for networks...\n";
    ScanFrame scan;
    scan.commit();
    
    mono::Debug << "Connecting to " << ssid << "\n";
    JoinFrame join(ssid, passphrase, secMode);
    join.commit();
    
    mono::Debug << "Getting IP address from DHCP...\n";
    SetIpParametersFrame ipparam;
    ipparam.commit();
    
    return true;
}











