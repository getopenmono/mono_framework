
#include "ip_address.h"
#include "stdio.h"
#include "string.h"

using namespace mono::net;

Ip4Address::Ip4Address()
{
    memset(addr, 0, 4);
}

Ip4Address::Ip4Address(const uint8_t addr[])
{
    memcpy(this->addr, addr, 4);
}

Ip4Address::Ip4Address(const char *ipaddress)
{
    strToNum(ipaddress);
}

Ip4Address::Ip4Address(const mono::String ipaddress)
{
    strToNum(ipaddress.stringData);
}

Ip4Address::Ip4Address(const Ip4Address &other)
{
    memcpy(addr, other.addr, 4);
}

Ip4Address &Ip4Address::operator=(const Ip4Address &other)
{
    memcpy(addr, other.addr, 4);

    return *this;
}

mono::String Ip4Address::toString() const
{
    return mono::String::Format("%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
}

void Ip4Address::strToNum(const char *str)
{
    int bytes[4];
    memset(bytes, 0, sizeof(int32_t)*4);
    sscanf(str, "%i.%i.%i.%i", &bytes[0], &bytes[1], &bytes[2], &bytes[3]);
    for (int i=0; i<4; i++) {
        addr[i] = bytes[i];
    }
}

bool Ip4Address::operator==(const Ip4Address &other) const
{
    if (this->addr[0] == other.addr[0] &&
        this->addr[1] == other.addr[1] &&
        this->addr[2] == other.addr[2] &&
        this->addr[3] == other.addr[3])
        return true;
    else
        return false;
}
