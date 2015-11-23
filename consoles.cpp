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


//void mono::error(const char *msg)
//{
//    defaultSerial.printf("%s\n", msg);
//}

void mono::memdump(void *ptr, uint32_t length)
{
    defaultSerial.printf("Memory Dump for 0x%x (dumping %i bytes)\n\r",ptr, length);
    char buffer[32];
    memset(buffer, 0, 32);
    
    char *byte = (char*) ptr;
    for (unsigned int i=0; i<length; i++) {
        snprintf(buffer, 32, "0x%x ",byte[i]);
        defaultSerial.printf((strlen(buffer)==4?"%s ":"%s"),buffer);
        if ((i+1) % 8 == 0) {
            defaultSerial.printf(" : %i\n\r",i+1);
        }
        else if ((i+1) % 4 == 0)
            defaultSerial.printf(" ");
    }
    
    if (length % 8 != 0)
        defaultSerial.printf("\n\r");
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
