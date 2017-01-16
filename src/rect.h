// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef __displaySimTest__rect__
#define __displaySimTest__rect__

#include "point.h"
#include "size.h"
#include "mn_string.h"

namespace mono { namespace geo {

    /**
     * @brief A Rectangle in a Cartesian  coordinate system, having a size and position
     * 
     * This class defines a geometric rectangle. It exists in a std. cartesian
     * coordinate system. It is defined by its upper left corner (X,Y), and a 
     * width and height.
     *
     * The rectangle cannot be rotated, its sides a parallel to the coordinate
     * system axis.
     *
     * It defines helper methods to calculate different positions and properties.
     * You can also extract interpolation information.
     *
     * @see Size
     * @see Point
     */
    class Rect : public Point, public Size
    {
    public:

        /**
         * @brief Construct a rectangle from position coordinates and size
         */
        Rect(int x, int y, int width, int height);

        /**
         * @brief Construct a rectangle from Point and Size objects
         */
        Rect(Point &p, Size &s);

        /**
         * @brief COnstruct an empty rectangle having position (0,0) and size (0,0)
         */
        Rect();
        
        Rect(const Rect &r);
        
        Rect& operator=(Rect r);
        
        Point Point() const;
        
        Size& Size();
        
        int X2() const;
        
        int Y2() const;

        /**
         * @brief Reutrn the position of the upper left corner
         *
         * This method is the same as casting the `Rect` to a `Point`
         * @return The point of the upper left corner
         */
        class Point UpperLeft() const;

        /**
         * @brief Reutrn the position of the lower right corner
         *
         * @return The point of the lower right corner
         */
        class Point LowerRight() const;

        /**
         * @brief Reutrn the position of the upper right corner
         *
         * @return The point of the upper right corner
         */
        class Point UpperRight() const;

        /**
         * @brief Reutrn the position of the lower left corner
         *
         * @return The point of the lower left corner
         */
        class Point LowerLeft() const;

        /**
         * @brief Return the position of the Rectangles center
         */
        class Point Center() const;

        /**
         * @brief Move (translate) the rectangle by its upper left corner
         */
        void setPoint(class Point p);

        /**
         * @brief Set a new size (width/height) of the rectangle
         *
         */
        void setSize(class Size s);
        
        /**
         * Check whether or not a Point is inside this rectangle.
         *
         */
        bool contains(class Point &p) const;

        bool contains(Rect const &other, bool equals = false) const;

        /**
         * Return this Rect cropped by the baundaries of another rect
         */
        Rect crop(Rect const &other) const;

        int Area();

        /**
         * @brief Return a string representation of the ractangle.
         * @return A string of the form `Rect( x, y, w, h )`
         */
        String ToString() const;

    };
    
} }

#endif /* defined(__displaySimTest__rect__) */
