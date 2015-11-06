//
//  point.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/07/15.
//
//

#ifndef __displaySimTest__point__
#define __displaySimTest__point__

#include <stdint.h>

namespace mono { namespace geo {
    
    /**
     * Class representing a point in a 2D cartisian coordinate system
     * The point has no width or height, omly an x-coordinate and a y-coordinate
     *
     * This class defines the coordinates, as well as method to manipulate the
     * point. Also functions for geometrical calculus is present.
     */
    class Point
    {
    protected:
        int x, y;
        
    public:
        Point();
        Point(int x1, int y1);
        Point(const Point &p);
        
        Point &operator=(const Point &p);
        
        //Getters
        int X() const;
        int Y() const;
        
        uint32_t Abs();
        
        //Setters
        void setX(int newX);
        void setY(int newY);
        void appendX(int addX);
        void appendY(int addY);
    };
    
    bool operator==(Point &p1, Point &p2);
    bool operator!=(Point &p1, Point &p2);
    
    Point operator-(Point &p1, Point &p2);
    
} }

#endif /* defined(__displaySimTest__point__) */
