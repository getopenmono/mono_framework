//
//  consoles.cpp
//  Mono
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//


#include "consoles.h"

using namespace mono;


void mono::error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

void mono::warning(const char *msg)
{
    printf("%s\n",msg);
}

Console::Console(FILE *f)
{
    fptr = f;
}


Console& Console::operator<<(const char *txt)
{
    fprintf(fptr, "%s",txt);
    return *this;
}

Console& Console::operator<<(int integer)
{
    fprintf(fptr, "%i",integer);
    return *this;
}

Console& Console::operator<<(char integer)
{
    fprintf(fptr, "%x",integer);
    return *this;
}

Console& Console::operator<<(float number)
{
    fprintf(fptr, "%f",number);
    return *this;
}

//Console& operator<<(Console &console, const char *txt)
//{
//    fprintf(console.fptr, "%s\n",txt);
//    return console;
//}
