// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "circle.h"


using namespace mono::geo;


// MARK: Contructors

Circle::Circle() : Point()
{
    radius = 0;
}

Circle::Circle(Point center, uint32_t radius) : Point(center)
{
    this->radius = radius;
}

Circle::Circle(int x, int y, uint32_t radius) : Point(x,y)
{
    this->radius = radius;
}


// MARK: GETTERS

uint32_t Circle::Radius() const
{
    return radius;
}
