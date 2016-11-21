// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

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
