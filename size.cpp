//
//  size.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/07/15.
//
//

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

int Size::Width()
{
    return this->width;
}

int Size::Height()
{
    return this->height;
}