
#include "tcp_server_socket.h"
#include <mbed_debug.h>

using namespace mono::net;

// MARK: Client connection methods

TcpServerSocket::ClientConnection::ClientConnection() :
    valid(false),
    serverSocket(0),
    clientDestination(),
    clientPort(0)
{
}

TcpServerSocket::ClientConnection::ClientConnection(const TcpServerSocket *serv, const Ip4Address &addr, const uint16_t port) :
    valid(true),
    serverSocket(serv),
    clientDestination(addr),
    clientPort(port)
{
}

bool TcpServerSocket::ClientConnection::write(const char *data, uint32_t length, const void *context)
{
    debug("should write to socket!");
    if (!valid || serverSocket->state != TcpServerSocket::SCK_LISTENING)
        return false;

    return MonoNetInterface::CurrentInterface->writeData(data, length, serverSocket->socketDescriptor, clientDestination.addr, clientPort);
}

TcpServerSocket::ClientData::ClientData(const char *data, uint32_t length,  ClientConnection *client) :
    client(client)
{
    this->data = data;
    this->length = length;
}

// MARK: Tcp Server Socket

TcpServerSocket::TcpServerSocket()
{
    state = SCK_INVALID;
    maxConns = 0;
    socketDescriptor = 0;
    listenPort = 0;
}

TcpServerSocket::TcpServerSocket(uint16_t prt, uint8_t maxConnections)
{
    listenPort = prt;
    maxConns = maxConnections;
    state = SCK_READY;
    socketDescriptor = 0;
}

// MARK: Methods

void TcpServerSocket::setState(SocketState newState)
{
    if (newState == state)
        return;

    state = newState;
    stateHandler.call(state);
}

bool TcpServerSocket::listen()
{
    if (state == SCK_READY || state == SCK_CLOSED)
    {
        if (MonoNetInterface::CurrentInterface == 0)
            return false;

        MonoNetInterface::CurrentInterface->createServerSocket(this, listenPort, maxConns);

        return true;
    }
    else
    {
        return false;
    }
}

void TcpServerSocket::close()
{
    if (SCK_LISTENING && MonoNetInterface::CurrentInterface != 0)
    {
        MonoNetInterface::CurrentInterface->closeSocket(this, socketDescriptor, listenPort);
    }
}

uint16_t TcpServerSocket::Port() const
{
    return listenPort;
}

TcpServerSocket::SocketState TcpServerSocket::State() const
{
    return state;
}

// MARK: Net Interface HAL methods callbacks

void TcpServerSocket::_onCreate(uint32_t descriptor, uint16_t localPort)
{
    debug("listening socket established!\r\n");
    socketDescriptor = descriptor;
    listenPort = localPort;

    setState(SCK_LISTENING);
}

void TcpServerSocket::_onConnectEstablish(uint32_t descriptor, const uint8_t *fromIp, uint16_t fromPort, uint16_t localPort)
{
    ClientConnection client(this, Ip4Address(fromIp), fromPort);
    clients.push_front(client);

    connectHandler.call(client);
}

void TcpServerSocket::_onData(const char *data, uint32_t length, uint8_t fromIp[], uint16_t fromPort)
{
    Ip4Address from(fromIp);

    for (std::list<ClientConnection>::iterator it=clients.begin();
         it != clients.end(); ++it)
    {
        if (it->clientDestination == from && it->clientPort == fromPort)
        {
            //call servers data handler
            ClientData cdata(data, length, &*it);
            dataHandler.call(cdata);

            // call the clients socket data handler
            ISocket::DataBuffer buffer;
            buffer.data = data;
            buffer.length = length;
            it->dataHandler.call(buffer);
            break;
        }
    }

}

void TcpServerSocket::_onClose(uint32_t descriptor, uint32_t sentBytes)
{
    debug("closed event\r\n");
    
    setState(SCK_CLOSED);
}

void TcpServerSocket::_onError(SocketError err)
{
    debug("socket error!\r\n");
    setState(SCK_ERROR);
}

