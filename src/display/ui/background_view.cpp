// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "background_view.h"

using namespace mono::ui;

BackgroundView::BackgroundView (display::Color color)
:
    View(geo::Rect(0,0,DisplayWidth(),DisplayHeight())),
    bgColor(color)
{
}

void BackgroundView::setBackgroundColor (display::Color color)
{
    bgColor = color;
}

mono::display::Color BackgroundView::Color() const
{
    return bgColor;
}

void BackgroundView::repaint ()
{
    painter.setBackgroundColor(bgColor);
    painter.drawFillRect(viewRect,true);
}
