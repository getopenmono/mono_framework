
#ifndef url_h
#define url_h

#include <mn_string.h>

namespace mono { namespace network {

    class Url : public String
    {
    protected:

        static int encodeUtf8Byte(uint8_t byte, char *encodedTxt);

        void initWithRaw(String url, bool encode);

    public:

        Url();
        Url(const String url, bool encode = true);
        Url(const char *url, bool encode = true);

        
        static String urlEncode(String url);
        static Url Format(const char *format, ...);
    };
} }

#endif /* url_h */
