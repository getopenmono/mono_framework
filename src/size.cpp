// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "size.h"

using namespace mono::geo;

Size::Size()
{
    this->width = this->height = 0;
}

Size::Size(int w, int h)
{
    this->width = w;
    this->height = h;
}

Size::Size(const Size &s)
{
    this->width = s.width;
    this->height = s.height;
}

Size& Size::operator=(const mono::geo::Size &s)
{
    this->width = s.width;
    this->height = s.height;
    return *this;
}

int Size::Width() const
{
    return this->width;
}

int Size::Height() const
{
    return this->height;
}

void Size::setWidth(int w)
{
    this->width = w;
}

void Size::setHeight(int h)
{
    this->height = h;
}