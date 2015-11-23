//
//  progress_bar_view.cpp
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 18/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "mono.h"
#include "progress_bar_view.h"

using namespace mono::ui;

void ProgressBarView::init()
{
    minValue = 0;
    maxValue = 100;
    currentValue = 0;
    barPixelLength = 0;
    borderColor = barColor = display::WhiteColor;
}

ProgressBarView::ProgressBarView() : View()
{
    init();
}

ProgressBarView::ProgressBarView(geo::Rect rect) : View(rect)
{
    init();
}

/// SETTERS

void ProgressBarView::setMaximum(int max)
{
    if (max == maxValue)
        return;
    
    if (max <= minValue)
        max = minValue + 1;
    
    maxValue = max;
    
    if (maxValue < currentValue)
        setValue(maxValue);
}

void ProgressBarView::setMinimum(int min)
{
    if (min == minValue)
        return;
    
    if (min >= maxValue)
        min = maxValue - 1;
    
    minValue = min;
    
    if (minValue > currentValue)
        setValue(minValue);
}

void ProgressBarView::setValue(int newValue)
{
    if (newValue == currentValue)
        return;
    
    if (newValue < minValue)
        newValue = minValue;
    
    if (newValue > maxValue)
        newValue = maxValue;
    
    currentValue = newValue;
    
    barPixelLength = ((1024*(viewRect.Width()-3)/(maxValue - minValue))*currentValue) / 1024;
    
    valueChangedHandler.call();
    
    scheduleRepaint();
}

/// GETTERS

int ProgressBarView::Maximum() const
{
    return maxValue;
}

int ProgressBarView::Minimum() const
{
    return minValue;
}

int ProgressBarView::CurrentValue() const
{
    return currentValue;
}

/// MISC

void ProgressBarView::repaint()
{
    //paint border
    geo::Size borderSize(viewRect.Width(), 6);
    geo::Rect border(viewRect, borderSize);
    
    painter.setForegroundColor(borderColor);
    painter.drawRect(border);
    
    geo::Rect bar(viewRect.X()+2,viewRect.Y()+2,barPixelLength,3);
    painter.setForegroundColor(barColor);
    painter.drawFillRect(bar);
    
}
