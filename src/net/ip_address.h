
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

        Ip4Address(const uint8_t addr[]);

        Ip4Address(const char *ipaddress);

        Ip4Address(const mono::String ipaddress);

        Ip4Address(const Ip4Address &other);

        Ip4Address &operator=(const Ip4Address &other);

        mono::String toString() const;

        bool operator==(const Ip4Address &other) const;

    };

} }

#endif /* ip_address_h */
