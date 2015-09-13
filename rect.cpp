//
//  rect.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 13/09/15.
//
//

#include "rect.h"

using namespace mono::geo;

Rect::Rect(int x, int y, int width, int height) : geo::Point(x,y), geo::Size(width, height)
{
}

Rect::Rect(geo::Point &p, geo::Size &s) : geo::Point(p), geo::Size(s)
{
}

Rect::Rect()
{
    this->x = this->y = this->width = this->height = 0;
}

Rect::Rect(const Rect &r)
{
    this->x = r.x;
    this->y = r.y;
    this->width = r.width;
    this->height = r.height;
}

Rect& Rect::operator=(Rect &r)
{
    this->x = r.x;
    this->y = r.y;
    this->width = r.width;
    this->height = r.height;
    return *this;
}

Point& Rect::Point()
{
    return *this;
}

Size& Rect::Size()
{
    return *this;
}

void Rect::setPoint(geo::Point p)
{
    this->x = p.X();
    this->y = p.Y();
}

void Rect::setSize(geo::Size s)
{
    this->width = s.Width();
    this->height = s.Height();
}

int Rect::Area()
{
    return Width() * Height();
}