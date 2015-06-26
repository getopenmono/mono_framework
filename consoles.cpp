//
//  consoles.cpp
//  Mono
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//


#include "consoles.h"
#include <Serial.h>

using namespace mono;


void mono::error(const char *msg)
{
    defaultSerial.printf("%s\n", msg);
}

void mono::warning(const char *msg)
{
    defaultSerial.printf("%s\n",msg);
}

Console::Console(mbed::Serial *ser)
{
    serial = ser;
}


Console& Console::operator<<(const char *txt)
{
    serial->printf("%s",txt);
    return *this;
}

Console& Console::operator<<(int integer)
{
    serial->printf("%i",integer);
    return *this;
}

Console& Console::operator<<(char integer)
{
    serial->printf("%x",integer);
    return *this;
}

Console& Console::operator<<(float number)
{
    serial->printf("%f",number);
    return *this;
}

//Console& operator<<(Console &console, const char *txt)
//{
//    fprintf(console.fptr, "%s\n",txt);
//    return console;
//}
