// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef _wifi_h
#define _wifi_h

#include <mono.h>
#include <wireless/module_communication.h>
#include <wireless/redpine_module.h>
#include <wifi_interface.h>

using namespace mono;

namespace mono { namespace io {

class Wifi : public IWifi, public mono::power::IPowerAware {
public:
    
    typedef redpine::Module::WifiSecurityModes SecurityMode;
    
//    enum NetworkEvents
//    {
//        CONNECTED,
//        CONNECT_ERROR,
//        DISCONNECTED
//    };
    
protected:
    
    mbed::SPI spi;
    redpine::ModuleSPICommunication spiComm;
    String ssid, passphrase;
    SecurityMode secMode;
    
    bool wifiInitInProgress;
    bool inited;
    bool connected;
    
    void convenienceInit();
    void moduleNetworkReady();
    void moduleNetworkError();
    
public:
    
    Wifi();
    
    Wifi(const String &ssid, const String &passphrase, const SecurityMode = redpine::Module::SEC_WPA_WPA2);
    
    bool connect();
    bool isConnected() const;
    
    // MARK: Power Aware Interface

    void onSystemWakeFromSleep();
};

} }

#endif
