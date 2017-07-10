// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "wifi.h"

using namespace mono::io;

Wifi::Wifi() :
    spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK),
    spiComm(spi, NC, RP_nRESET, RP_INTERRUPT)
{
    convenienceInit();
}

Wifi::Wifi(const String &ssid, const String &passphrase, const SecurityMode mode) :
    spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK),
    spiComm(spi, NC, RP_nRESET, RP_INTERRUPT)
{
    convenienceInit();
    this->ssid = ssid;
    this->passphrase = passphrase;
    this->secMode = mode;
}

// Internal methods

void Wifi::convenienceInit()
{
    wifiInitInProgress = false;
    inited = false;
    
    redpine::Module::setNetworkReadyCallback<Wifi>(this, &Wifi::moduleNetworkReady);
    redpine::Module::setConnectFailedCallback(this, &Wifi::moduleNetworkError);
}

void Wifi::moduleNetworkReady()
{
    inited = true;
    wifiInitInProgress = false;
    connected = true;
    
    networkReadyHandler.call();
    networkEventHandler.call(CONNECTED);
}

void Wifi::moduleNetworkError()
{
    inited = true;
    connected = false;
    wifiInitInProgress = false;
    
    networkErrorHandler.call();
    networkEventHandler.call(CONNECT_ERROR);
}

// MARK: Public methods

bool Wifi::connect()
{
    if (wifiInitInProgress || (inited && !connected))
        return false;
    else if (connected)
        return true;
    
    wifiInitInProgress = true;
    
    bool success = redpine::Module::initialize(&spiComm);
    if (!success)
        wifiInitInProgress = false;
    
    if (success)
        success = redpine::Module::setupWifiOnly(ssid, passphrase, secMode);
    
    if (!success)
        wifiInitInProgress = false;
    
    return success;
}

bool Wifi::isConnected() const
{
    return redpine::Module::IsNetworkReady();
}

// MARK: Power AWare interface methods

void Wifi::onSystemWakeFromSleep()
{
    wifiInitInProgress = false;
    inited = false;

    if (connected)
    {
        connected = false;
        disconnectHandler.call();
        networkEventHandler.call(DISCONNECTED);
    }
}
