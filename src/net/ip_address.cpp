
#include "ip_address.h"
#include "stdio.h"
#include "string.h"

using namespace mono::net;

Ip4Address::Ip4Address()
{
    memset(addr, 0, 4);
}

Ip4Address::Ip4Address(uint8_t addr[])
{
    memcpy(this->addr, addr, 4);
}

Ip4Address::Ip4Address(const mono::String ipaddress)
{
    sscanf(ipaddress.stringData, "%d.%d.%d.%d", &addr[0], &addr[1], &addr[2], &addr[3]);
}

mono::String Ip4Address::toString() const
{
    return mono::String::Format("%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
}
