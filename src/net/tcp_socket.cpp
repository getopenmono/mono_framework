
#include "tcp_socket.h"

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
    localPort = 0;
    state = SCK_READY;
}


bool TcpSocket::connect()
{
    if (!SCK_READY && !SCK_DISCONNECTED)
        return false;
}
