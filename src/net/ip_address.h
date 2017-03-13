
#ifndef ip_address_h
#define ip_address_h

#include <mn_string.h>

namespace mono { namespace net {

    class Ip4Address {
    protected:
        void strToNum(const char *str);

    public:
        uint8_t addr[4];

        Ip4Address();

        Ip4Address(uint8_t addr[]);

        Ip4Address(const char *ipaddress);

        Ip4Address(const mono::String ipaddress);

        mono::String toString() const;

    };

} }

#endif /* ip_address_h */
