//
//  string.cpp
//  analog
//
//  Created by Kristoffer Andersen on 13/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "mn_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "consoles.h"

using namespace mono;

String String::Format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    uint32_t size = vsnprintf(NULL, 0, format, args);
    String str(size+1);
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

//String::String(const char *format, ...)
//{
//    va_list args;
//    va_start(args, format);
//    va_
//    uint32_t size = snprintf(NULL,0, format, args);
//    preAllocbytes(size);
//    snprintf(stringData,size, format, args);
//    va_end(args);
//}

String::String(uint32_t preAllocBytes)
{
    preAllocbytes(preAllocBytes);
}

void String::CopyFromCString(const char * cstring)
{
    uint32_t strLen = strlen(cstring)+1; // plus string terminator
    malloced = true;
    stringData = (char*) malloc(strLen+sizeof(uint32_t));
    memset(stringData, 0, strLen+sizeof(uint32_t));
    strcpy(stringData, cstring);
    refCount = (uint32_t*) (stringData+strLen);
    *refCount = 1;
}

String::String(char *str, uint32_t length)
{
    malloced = true;
    stringData = (char*) malloc(length+sizeof(uint32_t));
    memcpy(stringData, str, length);
    refCount = (uint32_t*) (stringData+length);
    *refCount = 1;
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

char String::operator[](uint32_t pos) const
{
    return stringData[pos];
}

char* String::operator()() const
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
