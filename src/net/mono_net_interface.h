
#ifndef mono_net_interface_h
#define mono_net_interface_h

#include <stdint.h>

namespace mono { namespace net {

    class MonoNetInterface {
    public:

        // MARK: Internal Types & Classes

        class SocketContext {
        public:
            enum SocketError {
                CONNECT_ERROR
            };

            virtual void _onCreate(uint32_t descriptor, uint16_t localPort) = 0;
            virtual void _onData(const char *data, uint32_t length, uint8_t fromIp[], uint16_t fromPort) = 0;
            virtual void _onClose(uint32_t descriptor, uint32_t sentBytes) = 0;
            virtual void _onError(SocketError err) = 0;
        };

        // MARK: Public Action Methods

        virtual void createClientSocket(SocketContext *cnxt, uint8_t ip[], uint16_t destPort, uint16_t localPort, bool isUdp = false) = 0;

        virtual void createServerSocket(uint16_t port, bool isUdp = false) = 0;

        virtual void handleIncomingData(const char *data, uint32_t length, uint32_t sockDesc, uint8_t fromIp[], uint16_t fromPort) = 0;

        virtual void handleConnectEvent(uint32_t sockDesc) = 0;

        virtual bool writeData(const char *data, uint32_t length, uint32_t sockDesc, uint8_t ipAddr[], uint16_t destPort, bool isUdp = false) = 0;

        virtual void closeSocket(SocketContext *cnxt, uint32_t sockDesc, uint16_t destPort) = 0;

        // MARK: Static members

        static MonoNetInterface *CurrentInterface;
    };

} }

#endif /* mono_net_interface_hpp */
