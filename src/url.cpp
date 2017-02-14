
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

using namespace mono::network;
using mono::String;

Url::Url() : String()
{

}

Url::Url(const String url, bool encode) : String()
{
    initWithRaw(url, encode);
}

Url::Url(const char *url, bool encode) : String()
{
    initWithRaw(url, encode);
}

void Url::initWithRaw(String url, bool encode)
{
    Regex urlReg("\\?(.*)$");
    Regex::Capture cap[1];
    String encoded;
    if (encode && urlReg.Match(url, cap, 1))
    {
        String rawCapure = urlReg.Value(cap[0]);
        String encodedParams = urlEncode(rawCapure);
        int pathLen = url.Length() - rawCapure.Length();
        int parLen = encodedParams.Length();
        preAllocbytes(pathLen + parLen + 1);

        memcpy(stringData, url.stringData, pathLen);
        memcpy(stringData+pathLen, encodedParams.stringData, parLen + 1);
    }
    else
    {
        preAllocbytes(url.Length() + 1);
        memcpy(stringData, url.stringData, url.Length()+1);
    }
}

// MARK: Statics

int Url::encodeUtf8Byte(uint8_t byte, char *encodedTxt)
{
    snprintf(encodedTxt, 4, "%%%02X", byte) ; // %hh
    return 3;
}

String Url::urlEncode(String url)
{
    int len = url.Length();
    int newLen = 0;

    for (int i=0; i<len; i++) {
        uint8_t byte = url[i];

        // Valid chars -._~!()*.

        if ((byte >= '0' && byte <= '9') || // 0-9
            (byte >= 'A' && byte <= 'Z') || // A-Z
            (byte >= 'a' && byte <= 'z') || // a-z
             byte == '-' || byte == '.' || byte == '_' ||
             byte == '~' || byte == '!' || byte == '(' ||
             byte == ')' || byte == '*')
            newLen++;
        else if (byte > 0x7F)
        {
            //parse UTF-8 encoding
            if ((byte & 0xF0) == 0xF0) // 4 byte character
            {
                newLen += 4*3;
                i += 3;
            }
            else if ((byte & 0xE0) == 0xE0) // 3 byte char
            {
                newLen += 3*3;
                i += 2;
            }
            else if ((byte & 0xC0) == 0xc0) // 2 byte char
            {
                newLen += 2*3;
                i++;
            }
            else // 1 byte UTF-8 character
                newLen += 3;
        }
        else
            newLen += 3;
    }

    String escUrl(newLen);

    int indx = 0;
    int charPos = 0;
    while (indx < newLen) {
        uint8_t byte = url[charPos];

        if ((byte >= '0' && byte <= '9') || // 0-9
                 (byte >= 'A' && byte <= 'Z') || // A-Z
                 (byte >= 'a' && byte <= 'z') || // a-z
                 byte == '-' || byte == '.' || byte == '_' ||
                 byte == '~' || byte == '!' || byte == '(' ||
                 byte == ')' || byte == '*')
        {
            escUrl.stringData[indx] = byte;
            indx++;
            charPos++;
        }
        else if (byte > 0x7F)
        {
            //parse UTF-8 encoding
            if ((byte & 0xF0) == 0xF0) // 4 byte character
            {
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
            }
            else if ((byte & 0xE0) == 0xE0) // 3 byte char
            {
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
            }
            else if ((byte & 0xC0) == 0xc0) // 2 byte char
            {
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
            }
            else // 1 byte UTF-8 character
            {
                indx += encodeUtf8Byte(url[charPos++], escUrl.stringData+indx);
            }
        }
        else
        {
            indx += encodeUtf8Byte(byte, escUrl.stringData+indx);
            charPos++;
        }
    }

    return escUrl;
}

// MARK: Static Constructor

Url Url::Format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    uint32_t size = vsnprintf(0, 0, format, args);
    String str(size+1);
#ifdef __LP64__
    // Workaound for va_start and vsnprintf on 64bit OS X and *nix systems
    // See https://gist.github.com/foobit/4618064
    va_end(args);
    va_start(args, format);
#endif
    vsnprintf(str.stringData,size+1, format, args);

    return Url(str);
}
