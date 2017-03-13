
#ifndef tcp_socket_h
#define tcp_socket_h

#include "socket_interface.h"
#include "mono_net_interface.h"
#include "ip_address.h"

namespace mono { namespace net {

    class TcpSocket : ISocket {
    public:
        enum SocketState {
            SCK_READY,          /**< Socket is setup and ready to connect */
            SCK_CONNECTED,      /**< Socket is connect to remote host */
            SCK_DISCONNECTED,   /**< Socket has been disconnected from remote host */
            SCK_CONNECT_ERROR,  /**< Socket tried to connect to remote host, but failed */
            SCK_INVALID         /**< Socket is in an invalid state, cannot connect! */
        };

    protected:
        mbed::FunctionPointer connectHandler, disconnectHandler;

        Ip4Address destination;
        uint32_t destPort;
        uint32_t localPort;
        SocketState state;
        uint32_t socketDescriptor;

    public:

        TcpSocket();

        TcpSocket(Ip4Address address, uint16_t port);


        bool connect();


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

    };

} }

#endif /* tcp_socket_h */
