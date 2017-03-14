
#include "tcp_socket.h"
#include <stdio.h>
#include <mbed_debug.h>

using namespace mono::net;

TcpSocket::TcpSocket()
{
    destPort = localPort = 0;
    state = SCK_INVALID;
}

TcpSocket::TcpSocket(Ip4Address address, uint16_t port)
{
    destination = address;
    destPort = port;
    localPort = port;
    state = SCK_READY;
}

void TcpSocket::setState(mono::net::TcpSocket::SocketState newState, bool cb)
{
    if (newState != state)
    {
        state = newState;
        if (cb)
            stateHandler.call(state);
    }
}

bool TcpSocket::connect()
{
    if (!SCK_READY && !SCK_DISCONNECTED)
        return false;

    if (MonoNetInterface::CurrentInterface == 0)
        return false;

    MonoNetInterface::CurrentInterface->createClientSocket(this, destination.addr, destPort, localPort);

    return true;
}

void TcpSocket::close()
{
    if (state != SCK_CONNECTED)
        return;

    MonoNetInterface::CurrentInterface->closeSocket(this, socketDescriptor, destPort);
}

bool TcpSocket::write(const char *data, uint32_t length, const void *)
{
    if (state != SCK_CONNECTED)
        return false;

    return MonoNetInterface::CurrentInterface->writeData(data, length, socketDescriptor, destination.addr, destPort, false);
}

TcpSocket::SocketState TcpSocket::State() const
{
    return state;
}

Ip4Address TcpSocket::Destination() const
{
    return destination;
}

// MARK: Network HAL event handlers

void TcpSocket::_onCreate(uint32_t descriptor, uint16_t localPort)
{
    socketDescriptor = descriptor;
    this->localPort = localPort;

    setState(SCK_CONNECTED);
    connectHandler.call();
}

void TcpSocket::_onData(const char *data, uint32_t length, uint8_t *fromIp, uint16_t fromPort)
{
    DataBuffer buffer;
    buffer.data = data;
    buffer.length = length;

    dataHandler.call(buffer);
}

void TcpSocket::_onClose(uint32_t descriptor, uint32_t sentBytes)
{
    SocketState old = state;
    setState(SCK_DISCONNECTED);

    if (old != SCK_DISCONNECTED)
        disconnectHandler.call();
}

void TcpSocket::_onError(SocketError err)
{
    debug("Socket (%i) failed with error: %i\r\n", socketDescriptor, err);
    setState(SCK_CONNECT_ERROR);

    errorHandler.call();
}

