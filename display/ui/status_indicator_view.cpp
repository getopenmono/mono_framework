//
//  status_indicator_view.cpp
//  circleTest
//
//  Created by Kristoffer Andersen on 07/12/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#include "status_indicator_view.h"

using namespace mono::ui;

///MARK: Constructors

void StatusIndicatorView::initializer()
{
    state = false;
    onColor = display::EmeraldColor;
    offColor = display::SilverColor;
    borderColor = display::AsbestosColor;
}

StatusIndicatorView::StatusIndicatorView() : View()
{
    initializer();
}

StatusIndicatorView::StatusIndicatorView(geo::Rect vRct) : View(vRct)
{
    initializer();
}

StatusIndicatorView::StatusIndicatorView(geo::Rect vRct, bool status) : View(vRct)
{
    initializer();
    state = status;
}

///MARK: SETTERS

void StatusIndicatorView::setOffStateColor(display::Color col)
{
    offColor = col;
    scheduleRepaint();
}

void StatusIndicatorView::setOnStateColor(display::Color col)
{
    onColor = col;
    scheduleRepaint();
}

void StatusIndicatorView::setState(bool newState)
{
    if (state != newState)
    {
        state = newState;
        scheduleRepaint();
    }
}

///MARK: GETTERS

bool StatusIndicatorView::State() const
{
    return state;
}

/// MARK: Auxilliaries

void StatusIndicatorView::repaint()
{
    int r;
    if (viewRect.Width() < viewRect.Height())
        r = viewRect.Width();
    else
        r = viewRect.Height();
    
    geo::Circle lamp(viewRect, r);
    painter.setForegroundColor(state ? onColor : offColor);
    painter.drawFillCircle(lamp);
    
    painter.setForegroundColor(borderColor);
    painter.drawCircle(lamp);
}

