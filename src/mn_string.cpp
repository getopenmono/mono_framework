// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mn_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace mono;

String String::Format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    uint32_t size = vsnprintf(NULL, 0, format, args);
    String str(size+1);
#ifdef __LP64__
    // Workaound for va_start and vsnprintf on 64bit OS X and *nix systems
    // See https://gist.github.com/foobit/4618064
    va_end(args);
    va_start(args, format);
#endif
    vsnprintf(str.stringData,size+1, format, args);
    va_end(args);

    return str;
}

String::String()
{
    refCount = NULL;
    stringData = NULL;
    malloced = false;
}

String::String(uint32_t preAllocBytes)
{
    preAllocbytes(preAllocBytes);
}

void String::CopyFromCString(const char * cstring)
{
    // Make room for the string terminator.
    if (0 == cstring)
        preAllocbytes(1);
    else
    {
        uint32_t strLen = 1 + strlen(cstring);
        preAllocbytes(strLen);
        strcpy(stringData, cstring);
    }
}

String::String(char *str, uint32_t length)
{
    // Make room for the string terminator.
    preAllocbytes(length+1);
    memcpy(stringData, str, length);
}

String::String(char *str)
{
    CopyFromCString(str);
}

String::String(const char *str)
{
    CopyFromCString(str);
}

String::String(const String &str)
{
    if (str.malloced)
    {
        this->refCount = str.refCount;
        *(this->refCount) = *(this->refCount)+1;
    }
    else
    {
        refCount = NULL;
    }

    stringData = str.stringData;
    this->malloced = str.malloced;
}

uint32_t String::Length() const
{
    if (stringData != NULL)
        return strlen(stringData);
    else
        return 0;
}

String& String::operator=(const char *str)
{
    if (this != NULL)
        this->~String();

    stringData = (char*) str;
    malloced = false;
    refCount = NULL;

    return *this;
}

String& String::operator=(const String &str)
{
    if (this != NULL)
        this->~String();

    if (str.malloced)
    {
        this->refCount = str.refCount;
        *refCount = (*refCount) +1;
    }
    else
    {
        refCount = NULL;
    }

    this->stringData = str.stringData;
    this->malloced = str.malloced;

    return *this;
}

bool String::operator==(const mono::String &other) const
{
    if (this->stringData == 0 || other.stringData == 0)
        return false;

    return strcmp(this->stringData, other.stringData) == 0 ? true : false;
}

bool String::operator!=(const mono::String &other) const
{
    if (this->stringData == 0 || other.stringData == 0)
        return false;

    return strcmp(this->stringData, other.stringData) == 0 ? false : true;
}

bool String::operator==(const char *other) const
{
    if (this->stringData == 0)
        return false;

    return strcmp(this->stringData, other) == 0 ? true : false;
}

bool String::operator!=(const char *other) const
{
    if (this->stringData == 0)
        return false;
    
    return strcmp(this->stringData, other) == 0 ? false : true;
}

char String::operator[](uint32_t pos) const
{
    return stringData[pos];
}

char* String::operator()() const
{
    return CString();
}

char* String::CString() const
{
    return this->stringData;
}

String::~String()
{
    if (malloced)
    {
        *refCount = *refCount-1;
    }

    if (malloced && *refCount <= 0)
    {
        free(stringData);
    }

}

///////// PRIVATE METHODS

void String::preAllocbytes(uint32_t count)
{
    malloced = true;
    stringData = (char*) malloc(count+sizeof(uint32_t));
    memset(stringData, 0, count+sizeof(uint32_t));
    refCount = (uint32_t*) (stringData+count);
    *refCount = 1;
}
