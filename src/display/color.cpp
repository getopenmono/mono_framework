// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include <color.h>

using namespace mono::display;

// MARK: Public contructors

Color::Color()
{
    value = 0x0821; // black color
}

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
    value = ((R << 8) & 0xF800) |
            ((G << 3)  & 0x07E0) |
            ((B >> 3) & 0x1F);
}

// MARK: Getters

uint8_t Color::Red() const
{
    return (value >> 8) & 0xF8;
}
uint8_t Color::Green() const
{
    return (value >> 3) & 0xFC;
}
uint8_t Color::Blue() const
{
    return (value << 3) & 0xF8;
}

// MARK: Misc

uint8_t* Color::BytePointer()
{
    return (uint8_t*) &(this->value);
}

Color Color::scale(uint8_t factor) const
{
    if (factor == 0xFF)
        return *this;
    else if (factor == 0)
        return Color(0);
    
    uint16_t r = this->Red(), g = this->Green(), b = this->Blue();
    r *= factor;
    g *= factor;
    b *= factor;

    return Color(r >> 8, g >> 8, b >> 8);
}

Color Color::blendMultiply(Color other) const
{
    uint16_t r = this->Red(), g = this->Green(), b = this->Blue();
    r *= other.Red();
    g *= other.Green();
    b *= other.Blue();

    return Color(r >> 8, g >> 8, b >> 8);
}

Color Color::blendAdditive(Color other) const
{
    uint16_t r = this->Red(), g = this->Green(), b = this->Blue();
    r += other.Red();
    g += other.Green();
    b += other.Blue();

    return Color(r, g, b);
}


Color Color::invert() const
{
    uint8_t r = Red(), g = Green(), b = Blue();
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;

    return Color(r,g,b);
}

Color Color::alphaBlend(uint8_t intensity, Color const &other) const
{
    if (this->value == other.value)
        return other;
    
    Color srcA = this->scale(intensity);
    uint8_t dstA = (255 - intensity);
    Color blend = srcA + other.scale(dstA);
    return blend;
}

mono::String Color::toString() const
{
    return String::Format("(%u, %u, %u)", Red(), Green(), Blue());
}

// MARK: Operator overloads

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

Color Color::operator*(const mono::display::Color &col)
{
    return blendMultiply(col);
}

Color Color::operator+(const mono::display::Color &col)
{
    return blendAdditive(col);
}
