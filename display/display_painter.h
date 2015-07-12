//
//  display_painter.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 28/06/15.
//
//

#ifndef __displaySimTest__display_painter__
#define __displaySimTest__display_painter__

#include <stdint.h>
#include <color.h>
#include "display_controller_interface.h"

namespace mono { namespace display {
    
    /**
     * The DisplayPainter draws shapes on a display, using the
     * DisplayController interface.
     *
     * You should use this class to draw shapes on the screen, and use it
     * from inside view only. The standard view class has a reference to
     * to an instance of this class.
     *
     * The coordinate system used by the painter is the same as used by the 
     * display interface. This means all shape coords are relative to the display
     * origo at the top left corner for the screen, when screen is in portrait 
     * mode.
     *
     * A painter keeps track of an active foreground and background color. You
     * can set new colors at any time, and the succeeding draw calls will paint 
     * in that color. The standard draw color is the active foreground color,
     * but some draw routines might use the background color also. An example
     * is the font drawing routines.
     *
     * Like colors, the painter object keeps a active line width and textsize.
     * When drawing shapes based on lines @ref drawLine, @ref drawPolygon,
     * @ref drawRect and @ref drawEllipse, the line width used is the currently
     * active line width property of the painter object.
     *
     * When painting text characters the character size is dependend on the 
     * textsize property. Text painting is not affected by the current line width.
     *
     */
    class DisplayPainter
    {
    protected:
        IDisplayController *displayCtrl;
        
        Color foregroundColor, backgroundColor;
        uint8_t lineWidth, textSize;
        
        static const unsigned char font[];
        
        /**
         * Inline swap of two numbers.
         */
        void swap(uint16_t &a, uint16_t &b);
        
        int16_t abs(int16_t a);
        
    public:
        
        /**
         * Construct a new painter object that are attached to a display.
         * A painter object is automatically initialized by the view/UI system
         * and shared among the view classes.
         *
         * In most cases you should not have to initialize your own display painter.
         *
         * @param displayController A pointer to the display controller of the active display
         */
        DisplayPainter(IDisplayController *displayController);
        
        
        void setForegroundColor(Color color);
        
        void setBackgroundColor(Color color);
        
        
        uint8_t LineWidth() const;
        void setLineWidth(uint8_t w);
        
        uint8_t TextSize() const;
        void setTextSize(uint8_t size);
        
        
        /**
         * Draw a single pixel on a specific position on the display.
         *
         * The pixel will be the active foreground color, unless you set the 
         * third parameter to true.
         *
         * @param x The X-coordinate
         * @param y The Y coordinate
         * @param background Optional: Set to true to paint with active background color.
         */
        void drawPixel(uint16_t x, uint16_t y, bool background = false);
        
        /**
         * Paints a filled rectangle in the actuive foreground color. Coordinates
         * a defining the point of the rectangles upper left corner and are given
         * in screen coordinates. (Absolute coordinates)
         *
         * @brief Draw a filled rectangle
         * @param x X coordinate of upper left corner, in screen coordinates.
         * @param y Y coordinate of upper left corner, in screen coordinates.
         * @param width The width of the rectangle
         * @param height The height of the rectangle
         * @param background Optional: Set to `true` to paint in active background color
         */
        void drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool background = false);
        
        /**
         * Draw a line on the display. The line is defined by its two end points.
         * End point coordinates are in absolute screen coordinates.
         *
         * The line is _not_ anti-aliased.
         *
         * Based on Bresenham's algorithm. But If the line you intend to draw is
         * horizontal of vertical, this method will usemore efficient routines
         * specialized for these cases.
         *
         * @brief Draw a straight line between two points
         * @param x1 The X coordinate of the lines first endpoint
         * @param y1 The Y coordinate of the lines first endpoint
         * @param x2 The X coordinate of the lines second endpoint
         * @param y2 The Y coordinate of the lines second endpoint
         * @param background Optional: Set this to `true` to paint in active background color
         */
        void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool background = false);
        
        /**
         * Draw an outlined rectangle with the current line width and the active
         * color.
         *
         * @brief Draw a outlined rectangle
         * @param x Top left corner X coordinate
         * @param x Top left corner X coordinate
         * @param width The rectangles width
         * @param height The rectangles height
         * @param background Optional: Set this to `true` to paint in active background color
         */
        void drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool background = false);
        
        /**
         * Paint a single ASCII character on the display. Characters are always 
         * painted in the foreground color, with the background color as 
         * background fill.
         *
         * The character is painted in the currently selected text size. The text
         * is a monospaced font, width a minimum size of (5,3) per character.
         * The origo of a character is the upper left corner of the (5,3) 
         * rectangle.
         *
         * If you want write text on the screen, you should use the TextLabel 
         * view, or the Console view.
         *
         * @brief Draw a single character on the display
         * @param x The X coordinate of the characters upper left corner
         * @param y The Y coordinate of the characters upper left corner
         * @param character The text character to draw
         */
        void drawChar(uint16_t x, uint16_t y, char character);
        
        /**
         * Helper function to draw a vertical line very fast. This method uses
         * much less communication with the display.
         *
         * *This method is automatically called by @ref drawLine*
         *
         * @param x The lines X coordinate (same for both end points)
         * @param y1 The first end points Y coordinate
         * @param y2 The second end points Y coordinate
         * @param background Optional: Set this to `true` to paint in active background color
         */
        void drawVLine(uint16_t x, uint16_t y1, uint16_t y2, bool background = false);
        
        /**
         * Helper function to draw a horizontal line very fast. This method uses
         * much less communication with the display.
         *
         * *This method is automatically called by @ref drawLine*
         *
         * @param x1 The first end points X coordinate
         * @param x2 The second end points X coordinate
         * @param y The lines Y coordinate (same for both end points)
         * @param background Optional: Set this to `true` to paint in active background color
         */
        void drawHLine(uint16_t x1, uint16_t x2, uint16_t y, bool background = false);
    };
    
} }

#endif /* defined(__displaySimTest__display_painter__) */
