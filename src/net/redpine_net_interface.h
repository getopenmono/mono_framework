
#ifndef redpine_net_interface_h
#define redpine_net_interface_h

#include "mono_net_interface.h"
#include <redpine_module.h>
#include <redpine_command_frames.h>

namespace mono { namespace redpine {

    class RedpineSocketInterface : public net::MonoNetInterface
    {
        void createClientSocket(uint8_t ip[], uint16_t port, bool isUdp = false);

        void createServerSocket(uint16_t port, bool isUdp = false);

        void handleIncomingData(const char *data, uint32_t length, uint32_t sockDesc, uint8_t fromIp[], uint16_t fromPort);

        void handleConnectEvent(uint32_t sockDesc);

        void handleDisconnectEvent();

        void writeData(const char *data, uint32_t length, uint32_t sockDesc);
    };

} }

#endif /* redpine_net_interface_h */
