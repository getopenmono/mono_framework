//
//  color.cpp
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#include <color.h>

using namespace mono::display;

Color::Color(const int col)
{
    value = (uint16_t) col;
}

Color::Color(const Color &color)
{
    value = color.value;
}

uint16_t Color::operator=(Color col)
{
    value = col.value;
    return value;
}

bool Color::operator!=(const Color &col)
{
    return col.value != this->value;
}

bool Color::operator==(const Color &col)
{
    return this->value == col.value;
}
