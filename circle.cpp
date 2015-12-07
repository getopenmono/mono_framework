//
//  circle.cpp
//  circleTest
//
//  Created by Kristoffer Andersen on 07/12/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#include "circle.h"


using namespace mono::geo;


/// MARK: Contructors

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


/// MARK: GETTERS

uint32_t Circle::Radius() const
{
    return radius;
}