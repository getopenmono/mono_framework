//
//  circle.h
//  
//
//  Created by Kristoffer Andersen on 07/12/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#ifndef circle_h
#define circle_h

#include "point.h"

namespace mono { namespace geo {
    
    class Circle : public Point
    {
    protected:
        uint32_t radius;
        
    public:
        
        Circle();
        
        Circle(Point center, uint32_t radius);
        
        Circle(int x, int y, uint32_t radius);
        
        uint32_t Radius() const;
    };
    
} }

#endif /* circle_h */
