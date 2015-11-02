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

Color::Color(uint8_t R, uint8_t G, uint8_t B)
{
    value = ((R << 11) & 0xF800) |
            ((G << 5)  & 0x07E0) |
            (B & 0x1F);
}

uint16_t Color::operator=(Color col)
{
    value = col.value;
    return value;
}

uint8_t* Color::BytePointer()
{
    return (uint8_t*) &(this->value);
}

bool Color::operator!=(const Color &col)
{
    return col.value != this->value;
}

bool Color::operator==(const Color &col)
{
    return this->value == col.value;
}
