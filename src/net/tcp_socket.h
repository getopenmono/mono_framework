
#ifndef tcp_socket_h
#define tcp_socket_h

#include "socket_interface.h"
#include "mono_net_interface.h"
#include "ip_address.h"

namespace mono { namespace net {

    class TcpSocket : public ISocket, MonoNetInterface::SocketContext {
    public:
        enum SocketState {
            SCK_READY,          /**< Socket is setup and ready to connect */
            SCK_CONNECTED,      /**< Socket is connect to remote host */
            SCK_DISCONNECTED,   /**< Socket has been disconnected from remote host */
            SCK_CONNECT_ERROR,  /**< Socket tried to connect to remote host, but failed */
            SCK_INVALID         /**< Socket is in an invalid state, cannot connect! */
        };

    protected:
        mbed::FunctionPointer connectHandler, disconnectHandler, errorHandler;
        mbed::FunctionPointerArg1<void, SocketState> stateHandler;

        Ip4Address destination;
        uint16_t destPort;
        uint16_t localPort;
        SocketState state;
        uint32_t socketDescriptor;

        void setState(SocketState newState, bool triggerCallback = true);

    public:

        TcpSocket();

        TcpSocket(Ip4Address address, uint16_t port);

        bool connect();

        void close();

        virtual bool write(const char *data, uint32_t length, const void *context = 0);

        SocketState State() const;

        Ip4Address Destination() const;

        // MARK: Callback setters

        /**
         * @brief Set the connect event callback function
         * 
         */
        template <typename Context>
        void setConnectCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            connectHandler.attach<Context>(cnxt, memptr);
        }

        /**
         * @brief Set the *disconnect* event callback function
         * 
         */
        template <typename Context>
        void setDisconnectCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            disconnectHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setStateChangeCallback(Context *cnxt, void(Context::*memptr)(SocketState))
        {
            stateHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setErrorCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            errorHandler.attach<Context>(cnxt, memptr);
        }


        // MARK: Internal HAL interface methods (event handlers)

        virtual void _onCreate(uint32_t descriptor, uint16_t localPort);

        virtual void _onData(const char *data, uint32_t length, uint8_t fromIp[], uint16_t fromPort);

        virtual void _onClose(uint32_t descriptor, uint32_t sentBytes);

        virtual void _onError(SocketError err);

    };

} }

#endif /* tcp_socket_h */
