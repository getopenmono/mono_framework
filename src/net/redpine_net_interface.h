
#ifndef redpine_net_interface_h
#define redpine_net_interface_h

#include "mono_net_interface.h"

#include <redpine_command_frames.h>
#include <module_communication.h>
#include <FunctionPointer.h>

namespace mono { namespace redpine {

    class RedpineSocketInterface : public net::MonoNetInterface
    {
        static const int MaxAllowedSockets = 5;
    public:

        static RedpineSocketInterface current;

        class Command {
        public:
            ModuleFrame *frame;
            SocketContext *cnxt;

            void frameCompleted(ManagementFrame::FrameCompletionData *);

            void openFrameResponse(const OpenSocketFrame::socketFrameRcv *);

            void closeFrameResponse(const CloseSocketFrame::rsi_rsp_socket_close*);

        };

        // members

        static SocketContext *activeSockets[MaxAllowedSockets];
        ModuleCommunication::DataPayloadHandler dataFrameHandler;

        // MARK: interface methods

        RedpineSocketInterface();

        void createClientSocket(SocketContext *cnxt, uint8_t ip[], uint16_t destPort, uint16_t localPort, bool isUdp = false);

        void createServerSocket(uint16_t port, bool isUdp = false);

        void handleIncomingData(const char *data, uint32_t length, uint32_t sockDesc, uint8_t fromIp[], uint16_t fromPort);

        void handleConnectEvent(uint32_t sockDesc);

        void handleDisconnectEvent();

        bool writeData(const char *data, uint32_t length, uint32_t sockDesc,  uint8_t ipAddr[], uint16_t destPort, bool isUdp = false);

        void closeSocket(SocketContext *cnxt, uint32_t sockDesc, uint16_t destPort);

        // MARK: HAL methods

        void handleDataFrames(ModuleCommunication::DataPayload const &payload);
    };

} }

#endif /* redpine_net_interface_h */
