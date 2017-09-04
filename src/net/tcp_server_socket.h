
#ifndef tcp_server_socket_h
#define tcp_server_socket_h

#include "tcp_socket.h"
#include <list>
#include <FunctionPointer.h>

namespace mono { namespace net {

    class TcpServerSocket : public MonoNetInterface::SocketContext
    {
    public:

        class ClientConnection : public ISocket {
            friend TcpServerSocket;
        protected:
            bool valid;
        public:
            const TcpServerSocket *serverSocket;
            const Ip4Address clientDestination;
            const uint16_t clientPort;

            mbed::FunctionPointer disconnectHandler;

            ClientConnection();
            ClientConnection(const TcpServerSocket *serv, const Ip4Address &addr, const uint16_t port);

            bool write(const char *data, uint32_t length, const void *context = 0);
        };

        class ClientData : public ISocket::DataBuffer {
        public:
            ClientConnection *client;

            ClientData(const char *data, uint32_t length, ClientConnection*);
        };

        enum SocketState {
            SCK_READY,
            SCK_LISTENING,
            SCK_CLOSED,
            SCK_ERROR,
            SCK_INVALID
        };

    protected:

        mbed::FunctionPointerArg1<void, SocketState> stateHandler;
        mbed::FunctionPointerArg1<void, ClientConnection&> connectHandler;
        mbed::FunctionPointerArg1<void, ClientData&> dataHandler;

        uint8_t maxConns;
        std::list<ClientConnection> clients;
        SocketState state;
        uint16_t listenPort;
        uint32_t socketDescriptor;

        void setState(SocketState newState);

    public:

        TcpServerSocket();

        TcpServerSocket(uint16_t port, uint8_t maxConnections = 10);

        // MARK: Methods

        bool listen();

        void close();

        uint16_t Port() const;

        SocketState State() const;

        // MARK: Callback Methods

        template <typename Context>
        void setConnectAcceptCallback(Context *cnxt, void(Context::*memptr)(ClientConnection&))
        {
            connectHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setStateChangeCallback(Context *cnxt, void(Context::*memptr)(SocketState))
        {
            stateHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setDataCallback(Context *cnxt, void(Context::*memptr)(ClientData&))
        {
            dataHandler.attach<Context>(cnxt, memptr);
        }


        // MARK: Net Interface HAL methods callbacks

        virtual void _onCreate(uint32_t descriptor, uint16_t localPort);
        virtual void _onConnectEstablish(uint32_t descriptor, const uint8_t fromIp[], uint16_t fromPort, uint16_t localPort);
        virtual void _onData(const char *data, uint32_t length, uint8_t fromIp[], uint16_t fromPort);
        virtual void _onClose(uint32_t descriptor, uint32_t sentBytes);
        virtual void _onError(SocketError err);
    };

} }

#endif /* tcp_server_socket_h */
