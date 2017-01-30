// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "display_painter.h"
#include "glcdfont.h"
#include <us_ticker_api.h>
#include <mbed_debug.h>
#include <math.h>

using namespace mono::display;
using namespace mono;

DisplayPainter::DisplayPainter(IDisplayController *dispctrl, bool assignRefreshHandler) : foregroundColor(WetAsphaltColor), backgroundColor(CloudsColor)
{
    displayCtrl = dispctrl;
    antiAliasing = false;

//    if (displayCtrl != NULL)
//        displayCtrl->AddRefreshCallback(&displayRefreshHandler);
    if (assignRefreshHandler)
        displayCtrl->setRefreshHandler(&displayRefreshHandler);
    
    lineWidth = 1;
    textSize = 1;
}

DisplayPainter::~DisplayPainter()
{
//    if (displayCtrl != NULL)
//        displayCtrl->RemoveRefreshCallback(&displayRefreshHandler);
}

/// MARK: Color Accessors

void DisplayPainter::setForegroundColor(Color color)
{
    foregroundColor = color;
}

void DisplayPainter::setBackgroundColor(Color color)
{
    backgroundColor = color;
}

Color DisplayPainter::ForegroundColor() const
{
    return foregroundColor;
}

Color DisplayPainter::BackgroundColor() const
{
    return backgroundColor;
}

/// MARK: Anti-aliasing Accessors

void DisplayPainter::useAntialiasedDrawing(bool enable)
{
    antiAliasing = enable;
}

bool DisplayPainter::IsAntialiasedDrawing()
{
    return antiAliasing;
}

/// MARK: Pencil Property Accessors

uint8_t DisplayPainter::LineWidth() const
{
    return lineWidth;
}

void DisplayPainter::setLineWidth(uint8_t w)
{
    lineWidth = w;
}

uint8_t DisplayPainter::TextSize() const
{
    return textSize;
}

void DisplayPainter::setTextSize(uint8_t size)
{
    textSize = size;
}

/// MARK: Painting Canvas Info Accessors

uint16_t DisplayPainter::CanvasWidth() const
{
    return displayCtrl->ScreenWidth();
}

uint16_t DisplayPainter::CanvasHeight() const
{
    return displayCtrl->ScreenHeight();
}

IDisplayController* DisplayPainter::DisplayController() const
{
    return displayCtrl;
}

/// MARK: Drawing methods

void DisplayPainter::drawPixel(uint16_t x, uint16_t y, bool bg)
{
    displayCtrl->setWindow(x, y, 1, 1);
    displayCtrl->write(bg ? backgroundColor : foregroundColor);
}

void DisplayPainter::drawPixel(geo::Point const &pos, bool background)
{
    drawPixel(pos.X(), pos.Y(), background);
}

void DisplayPainter::drawPixel(uint16_t x, uint16_t y, uint8_t intensity, bool bg)
{
    displayCtrl->setWindow(x, y, 1, 1);
    Color c;
    if (bg)
        c = backgroundColor.alphaBlend(intensity, foregroundColor);
    else
        c = foregroundColor.alphaBlend(intensity, backgroundColor);

    displayCtrl->write(c);
}

/// MARK: Rects

void DisplayPainter::drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool bg)
{
    drawVLine(x, y, y+height-1, bg);
    drawVLine(x+width-1, y, y+height-1, bg);
    drawHLine(x, x+width-1, y, bg);
    drawHLine(x, x+width, y+height-1, bg);
}

void DisplayPainter::drawRect(geo::Rect const &rct, bool background)
{
    drawRect(rct.X(), rct.Y(), rct.Width(), rct.Height(), background);
}

void DisplayPainter::drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool bg)
{
    displayCtrl->setWindow(x, y, width, height);
    //displayCtrl->setCursor(x, y);

    Color color = bg ? backgroundColor : foregroundColor;
    uint32_t size = width*height;
    for (uint32_t i=0; i<size; i++) {
        displayCtrl->write(color);
    }
}

void DisplayPainter::drawFillRect(geo::Rect const &rct, bool background)
{
    drawFillRect(rct.X(), rct.Y(), rct.Width(), rct.Height(), background);
}

/// MARK: Lines

void DisplayPainter::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool bg)
{
    //use VLine or HLine if possible
    if (x0==x1)
        return drawVLine(x0, y0, y1, bg);
    else if (y0==y1)
        return drawHLine(x0, x1, y0, bg);

    if (antiAliasing)
        return drawAALine(x0, y0, x1, y1, bg);

    // Use Bresenham's
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, bg);
        } else {
            drawPixel(x0, y0, bg);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void DisplayPainter::drawLine(geo::Point const &from, geo::Point const &to, bool background)
{
    drawLine(from.X(), from.Y(), to.X(), to.Y(), background);
}

inline uint8_t aa_line_fpart(uint32_t number)
{
    return 0x000000FF & number;
//    if (number < 0.0)
//        return 1.0 - (number - (int)number);
//    else
//        return number - (int)number;
}

inline uint8_t aa_line_rfpart(uint32_t number)
{
    return 255 - aa_line_fpart(number);
}

void DisplayPainter::drawAALine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool bg)
{

    // Use Xiaolin Wu's line algorithm
    bool steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    float gradient = 1.0*dy / dx;
    int32_t gradientInt = round(gradient*256.0);
    int32_t intery = (y0 << 8) + gradientInt;
    
    // 45 degree line smooth correction
    uint8_t corr = 0;
	float aG = gradient > 0 ? gradient : -gradient;
    if (aG == 1.0)
    {
        corr = 255*0.16;
    }

    // main loop
    for (uint16_t x=x0; x<x1; x++)
    {
        uint8_t i1 = aa_line_rfpart(intery);
        uint8_t i2 = aa_line_fpart(intery);
        int16_t rawInt = intery / 256;
        if (steep) {
            drawPixel(     rawInt, x, i1, bg);
            drawPixel(   rawInt+1, x, i2 + corr, bg);
        } else {
            drawPixel(x,       rawInt, i1, bg);
            drawPixel(x,     rawInt+1, i2 + corr, bg);

            if (corr)
            {
                drawPixel(x, rawInt-1, i2 + corr, bg);
            }
        }

        intery = intery + gradientInt;
    }
}



void DisplayPainter::drawAALine(geo::Point const &from, geo::Point const &to, bool background)
{
    drawAALine(from.X(), from.Y(), to.X(), to.Y(), background);
}

void DisplayPainter::drawVLine(uint16_t x, uint16_t y1, uint16_t y2, bool bg)
{
    if (y1 > y2)
        swap(y1, y2);

    displayCtrl->setWindow(x, y1, 1, y2-y1);
    for (int i=0; i<y2-y1; i++)
    {
        displayCtrl->write(bg ? backgroundColor : foregroundColor);
    }
}

void DisplayPainter::drawHLine(uint16_t x1, uint16_t x2, uint16_t y, bool bg)
{
    if (x1 > x2)
        swap(x1, x2);

    displayCtrl->setWindow(x1, y, x2-x1, 1);
    for (int i=0; i<x2-x1; i++)
    {
        displayCtrl->write(bg ? backgroundColor : foregroundColor);
    }
}

/// MARK: Simple Characters

void DisplayPainter::drawChar(uint16_t x, uint16_t y, char character)
{
    if((x >= displayCtrl->ScreenWidth())    || // Clip right
       (y >= displayCtrl->ScreenHeight())   || // Clip bottom
       ((x + 6 * textSize - 1) < 0)         || // Clip left
       ((y + 8 * textSize - 1) < 0))           // Clip top
        return;


    for (int8_t i=0; i<6; i++ ) {
        uint8_t line;

        if (i == 5)
            line = 0x0;
        else
            line = *(font+(character*5)+i);

        // only relevant for textSize = 1
        this->displayCtrl->setWindow(x+i, y, 1, 8);

        for (int8_t j = 0; j<8; j++) {

            if (line & 0x1) {
                if (textSize == 1) // default size
                    displayCtrl->write(foregroundColor);
                    //drawPixel(x+i, y+j);
                else {  // big size
                    drawFillRect(x+(i*textSize), y+(j*textSize), textSize, textSize);
                }
            } else if (backgroundColor != foregroundColor) {
                if (textSize == 1) // default size
                    displayCtrl->write(backgroundColor);
                    //drawPixel(x+i, y+j, true);
                else {  // big size
                    drawFillRect(x+i*textSize, y+j*textSize, textSize, textSize, true);
                }
            }

            line >>= 1;
        }
    }
}

/// MARK: Circles

void DisplayPainter::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, bool color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void DisplayPainter::drawCircle(geo::Circle const &circle, bool bg)
{
    drawCircle(circle.X(), circle.Y(), circle.Radius(), bg);
}

void DisplayPainter::drawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, bool color)
{
    drawVLine(x0, y0-r, y0-r+2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void DisplayPainter::drawFillCircle(geo::Circle const &circle, bool bg)
{
    drawFillCircle(circle.X(), circle.Y(), circle.Radius(), bg);
}

void DisplayPainter::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, bool color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            drawVLine(x0+x, y0-y, y0-y+2*y+1+delta, color);
            drawVLine(x0+y, y0-x, y0-x+2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            drawVLine(x0-x, y0-y, y0-y+2*y+1+delta, color);
            drawVLine(x0-y, y0-x, y0-x+2*x+1+delta, color);
        }
    }
}

/// MARK: Simple Helper Methods

void DisplayPainter::swap(uint16_t &a, uint16_t &b)
{
    uint16_t t = a;
    a = b;
    b = t;
}

int16_t DisplayPainter::abs(int16_t a)
{
    return a<0 ? -a : a;
}
