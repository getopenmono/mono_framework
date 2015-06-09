//
//  redpine_module.h
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//

#ifndef __spiTest__redpine_module__
#define __spiTest__redpine_module__

#include <SPI.h>
#include "spi_commands.h"
#include "module_communication.h"
#include "redpine_command_frames.h"

namespace mono { namespace redpine {
    
    class Module
    {
        friend ModuleFrame;
        friend ManagementFrame;
    public:
        
        /**
         * The module can operate in different modes, that controls the different
         * wireless capabilities.
         */
        enum ModuleCoExistenceModes
        {
            MODE_WIFI_ONLY,         /**< Only enabled Wifi, with embedded TCP/IP */
            MODE_BT_CLASSIC,        /**< Only Bluetooth Classic (2.0) is enabled */
            MODE_BLE,               /**< Only Bluetooth LE is enabled */
            MODE_WIFI_BT_CLASSIC,   /**< Coex-mode with wifi and BT classic */
            MODE_WIFI_BLE           /**< Coex-mode with wifi and BLE */
        };
        
        /**
         * List of Wifi security modes. Use these when you connect to an access 
         * point.
         *
         * The last option `SEC_WPA_WPA2` is a mixed mode, that gives priority
         * to WPA2.
         */
        enum WifiSecurityModes
        {
            SEC_OPEN_NETWORK,       /**< No security */
            SEC_WPA,
            SEC_WPA2,
            SEC_WEP,
            SEC_ENTERPRISE_WPA,
            SEC_ENTERPRISE_WPA2,
            SEC_WPA_WPA2            /**< Either in WPA/WPA2 mode (Mixed mode) */
        };
        
    protected:

        /** The only instantiation of the module class */
        static Module moduleSingleton;
        
        /** True is the module communication interface is initialized */
        bool communicationInitialized;
        
        /** True if the module has its network stack ready */
        bool networkInitialized;
        
        /** The communication interface used by the module */
        ModuleCommunication *comIntf;
        
        /** This holds the module currently initalized operation mode */
        ModuleCoExistenceModes OperatingMode;
        
        /**
         * Protected class constructor
         * Can only be called by the Instance() method.
         *
         */
        Module();
        
    public:
        
        /**
         * Obtain a reference to the singleton module object
         */
        static Module* Instance();
        
        /**
         * Send initialize the module and set the communication interface
         * The interface will also be initialized by this method.
         *
         * @param commInterface The module communication interface to be used
         */
        static bool initialize(ModuleCommunication *commInterface);
        
        /**
         * Initializes the module in Eifi only mode with enabled TCP/IP stack
         * We expect this mode to be the defult mode, when only using Wifi
         *
         * The method will set the module in Wifi client mode with the 2.4 GHz
         * band antenna.
         *
         * @param ssid The SSID name of access point to connect to
         * @param passphrase The passphrase of the access point to join
         * @param secMode The access points security setting, default is WPA/WPA2
         */
        static bool setupWifiOnly(const char *ssid, const char *passphrase, WifiSecurityModes secMode = SEC_WPA_WPA2);
    };
    
    
    
    
    
}
}

#endif /* defined(__spiTest__redpine_module__) */
