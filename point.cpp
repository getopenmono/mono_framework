//
//  point.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/07/15.
//
//

#include "point.h"

#include <math.h>

using namespace mono::geo;

Point::Point()
{
    this->x = this->y = 0;
}

Point::Point(int x1, int y1)
{
    this->x = x1;
    this->y = y1;
}

Point::Point(const Point &p)
{
    this->x = p.x;
    this->y = p.y;
}

Point& Point::operator=(const Point &p)
{
    this->x = p.x;
    this->y = p.y;
    return *this;
}

int Point::X() const
{
    return this->x;
}

int Point::Y() const
{
    return this->y;
}

void Point::setX(int newX)
{
    this->x = newX;
}

void Point::setY(int newY)
{
    this->y = newY;
}

void Point::appendX(int addX)
{
    this->x += addX;
}

void Point::appendY(int addY)
{
    this->y += addY;
}

uint32_t Point::Abs()
{
    return sqrtf(powf(this->x, 2)+powf(this->y, 2));
}

bool mono::geo::operator==(mono::geo::Point &p1, mono::geo::Point &p2)
{
    return (p1.X() == p2.X() && p1.Y() == p2.Y());
}

bool mono::geo::operator!=(mono::geo::Point &p1, mono::geo::Point &p2)
{
    return (p1.X() != p2.X() || p1.Y() != p2.Y());
}

Point mono::geo::operator-(mono::geo::Point &p1, mono::geo::Point &p2)
{
    return geo::Point(p1.X() - p2.X(), p1.Y() - p2.Y());
}