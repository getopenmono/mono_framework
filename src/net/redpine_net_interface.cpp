
#include "redpine_net_interface.h"

using namespace mono::redpine;

mono::net::MonoNetInterface::CurrentInterface = RedpineSocketInterface();

void RedpineSocketInterface::createClientSocket(uint8_t ip[], uint16_t port, bool isUdp = false)
{
    Open
}

void RedpineSocketInterface::createServerSocket(uint16_t port, bool isUdp = false)
{

}

void RedpineSocketInterface::handleIncomingData(const char *data, uint32_t length,
                                                uint32_t sockDesc, uint8_t fromIp[],
                                                uint16_t fromPort)
{

}

void RedpineSocketInterface::handleConnectEvent(uint32_t sockDesc)
{

}

void RedpineSocketInterface::handleDisconnectEvent()
{

}

void RedpineSocketInterface::writeData(const char *data, uint32_t length, uint32_t sockDesc)
{
    
}
