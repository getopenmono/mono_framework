
#ifndef mono_net_interface_h
#define mono_net_interface_h

#include <stdint.h>

namespace mono { namespace net {

    class MonoNetInterface {
    public:
        virtual void createClientSocket(uint8_t ip[], uint16_t port, bool isUdp = false) = 0;

        virtual void createServerSocket(uint16_t port, bool isUdp = false) = 0;

        virtual void handleIncomingData(const char *data, uint32_t length, uint32_t sockDesc, uint8_t fromIp[], uint16_t fromPort) = 0;

        virtual void handleConnectEvent(uint32_t sockDesc) = 0;

        virtual void handleDisconnectEvent() = 0;

        virtual void writeData(const char *data, uint32_t length, uint32_t sockDesc) = 0;


        static MonoNetInterface *CurrentInterface;
    };

} }

#endif /* mono_net_interface_hpp */
