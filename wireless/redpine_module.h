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
#include <power_aware_interface.h>
#include <queue.h>

namespace mono { namespace redpine {
    
    /**
     * Redpine Wireless module Class. The module class controls the physical
     * module, sending and receiving data from the device. It utilizes a
     * communication interface (@ref ModuleCommunication) to send and receive
     * data. This makes the class independent of the communication protocol.
     *
     * Because there is only one physical module, this class is a singleton 
     * object. You access it by the static @ref Instance method. To setup the
     * Redpine module you must call the static global methods:
     * 
     * 1. **@ref Initialize**: Initializes the communication interface
     * 2. **@ref setupWifiOnly**: Join a wifi network with security and setup DHCP
     *
     * Only networking classes should access the module directly. Tcp socket, 
     * DNS resolution etc, should be handled by dedicated classes, that uses 
     * this class.
     */
    class Module : power::IPowerAware
    {
        friend ModuleFrame;
        friend ManagementFrame;
        friend AppController;
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
        
        /**
         * Command messages that can be written to the Bootloaders host interface
         * register.
         */
        enum BootloaderMessageCodes
        {
            HOST_INTERACT_REG_VALID = 0xAB00,
            RSI_LOAD_IMAGE_I_FW = 0x0031,
            RSI_ENABLE_BOOT_BYPASS = 0x0037
        };
        
        /**
         * Memory addresses of the registers related the Bootloader
         *
         */
        enum BootloaderRegisters
        {
            HOST_INTF_REG_OUT = 0x4105003C,
            HOST_INTF_REG_IN = 0x41050034
        };
        
        
        enum ModulePowerState
        {
            FULL_AWAKE      = 0x01,
            ULTRA_LOW_SLEEP = 0x02,
            LOW_SLEEP       = 0x04,
            ACTIVE_SLEEP    = 0x08
        };
        
        /** Object to configure static IP */
        typedef struct
        {
            uint8_t ipAddress[4];   /**< IPv4 client address */
            uint8_t netmask[4];     /**< IPv4 netmask */
            uint8_t gateway[4];     /**< IPv4 Gateway */
        } StaticIPParams;
        
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
        
        /** The current state of the module, is it awake or sleeping */
        ModulePowerState CurrentPowerState;
        
        /** A queue over the pending requests to-be-send to the module */
        GenericQueue<ManagementFrame> requestFrameQueue;
        
        /** A queue over the pending requests sent / pending to the module */
        GenericQueue<ManagementFrame> responseFrameQueue;
        
        /** User can install a network ready event callback in this handler */
        mbed::FunctionPointer networkReadyHandler;
        
        mbed::FunctionPointerArg1<void, StaticIPParams*> staticIPHandler;
        
        uint8_t ipAddress[4], netmask[4], gateway[4], macAddress[6];
        String hostname;
        
        bool dhcpEnabled;
        
        /**
         * Protected class constructor
         * Can only be called by the Instance() method.
         *
         */
        Module();
        
        /**
         * Handle the modules periodic wake ups
         * Puts the module to sleep again, if no input is to be sent.
         */
        void handleSleepWakeUp();
        
        /** Callback for the DHCP Mgmt frame response, that indicate the network is ready */
        void onNetworkReady(ManagementFrame::FrameCompletionData *data);
        
        void onSystemPowerOnReset();
        
        void onSystemEnterSleep();

        void onSystemWakeFromSleep();
        
        void OnSystemBatteryLow();
        
    public:
        
        /**
         * Callback function installed into the CommunicationInterface interrupt
         * callback listener.
         */
        void moduleEventHandler();
        
        /**
         * Obtain a reference to the singleton module object
         */
        static Module* Instance();
        
        /**
         * Sends the inirialize command and waits for Card Ready from the module
         * and set the communication interface for this object. After this method
         * is called the module is ready to receive the OperMode command etc.
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
        static bool setupWifiOnly(String ssid, String passphrase, WifiSecurityModes secMode = SEC_WPA_WPA2);
        
        /**
         * Check to see if the module has a ready network stack with initialized
         * IP configuration. A ready network stack is ready to fetch or send data
         * to and from the internet.
         *
         * @brief See id the network is ready and running
         * @returns `true` if the network is ready, `false` otherwise
         */
        static bool IsNetworkReady();
        
        
        template <typename Owner>
        static void setNetworkReadyCallback(Owner *obj, void (Owner::*memPtr)(void))
        {
            Module::Instance()->networkReadyHandler.attach<Owner>(obj, memPtr);
        }
        
        template <typename Owner>
        static void setStaticIPCallback(Owner *obj, void (Owner::*memPtr)(StaticIPParams*))
        {
            Module::Instance()->staticIPHandler.attach<Owner>(obj, memPtr);
        }
    };
    
    
    
    
    
}
}

#endif /* defined(__spiTest__redpine_module__) */
