//
//  rect.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 13/09/15.
//
//

#ifndef __displaySimTest__rect__
#define __displaySimTest__rect__

#include "point.h"
#include "size.h"

namespace mono { namespace geo {
    
    class Rect : public Point, public Size
    {
    public:
        
        Rect(int x, int y, int width, int height);
        
        Rect(Point &p, Size &s);
        
        Rect();
        
        Rect(const Rect &r);
        
        Rect& operator=(Rect &r);
        
        
        Point& Point();
        
        Size& Size();
        
        void setPoint(class Point p);
        
        void setSize(class Size s);
        
        
        int Area();
    };
    
} }

#endif /* defined(__displaySimTest__rect__) */
