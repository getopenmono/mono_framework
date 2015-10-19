//
//  display_painter.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 28/06/15.
//
//

#include "display_painter.h"
#include "glcdfont.h"
#include <math.h>

using namespace mono::display;

DisplayPainter::DisplayPainter(IDisplayController *dispctrl) : foregroundColor(WhiteColor), backgroundColor(BlackColor)
{
    displayCtrl = dispctrl;
    
    lineWidth = 1;
    textSize = 1;
    
    //memcpy(currentMatrix, identityMatrix, sizeof(float32_t)*9);
    
    
}

void DisplayPainter::setForegroundColor(Color color)
{
    foregroundColor = color;
}

void DisplayPainter::setBackgroundColor(Color color)
{
    backgroundColor = color;
}

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

float32_t* DisplayPainter::CurrentMatrix()
{
    return currentMatrix;
}


void DisplayPainter::ResetTransformation()
{
    //memcpy(currentMatrix, identityMatrix, sizeof(float32_t)*9);
}

void DisplayPainter::setRotation(int degrees)
{
    float32_t rads = degrees/360.0 * 2*3.1428;
    currentMatrix[0] = cosf(rads);
    currentMatrix[1] = -sinf(rads);
    currentMatrix[3] = sinf(rads);
    currentMatrix[4] = cosf(rads);
}

void DisplayPainter::setOrigin(int16_t x, int16_t y)
{
    currentMatrix[2] = x;
    currentMatrix[5] = y;
}

void DisplayPainter::setScaling(uint16_t percent)
{
    
}


void DisplayPainter::drawPixel(uint16_t x, uint16_t y, bool bg)
{
    //apply transformation matrix
    int16_t x1 = x;//x*currentMatrix[0] + y*currentMatrix[1] + 1*currentMatrix[2];
    int16_t y1 = y;//x*currentMatrix[3] + y*currentMatrix[4] + 1*currentMatrix[5];
    
    displayCtrl->setWindow(x1, y1, 1, 1);
    displayCtrl->write(bg ? backgroundColor : foregroundColor);
}

void DisplayPainter::drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool bg)
{
    drawLine(x, y, y+height, bg);
    drawLine(x+width, y, y+height, bg);
    drawLine(x, x+width, y, bg);
    drawLine(x, x+width, y+height, bg);
}

void DisplayPainter::drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool bg)
{
    displayCtrl->setWindow(x, y, width, height);
    //displayCtrl->setCursor(x, y);
    
    for (int i=0; i<width*height; i++) {
        displayCtrl->write(bg ? backgroundColor : foregroundColor);
    }
}

void DisplayPainter::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool bg)
{
    //use VLine or HLine if possible
    if (x0==x1)
        return drawVLine(x0, y0, y1, bg);
    else if (y0==y1)
        return drawHLine(x0, x1, y0, bg);
    
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
            } else {
                //this->displayCtrl->setCursor(x+i, y+j+1);
            }
            
            
            line >>= 1;
        }
    }
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
