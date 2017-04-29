// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "abstract_button_view.h"

using namespace mono::ui;

AbstractButtonView::AbstractButtonView() : ResponderView() {}

AbstractButtonView::AbstractButtonView(geo::Rect rct) : ResponderView(rct) {}


// MARK: Touch Handlers

void AbstractButtonView::TouchBegin(mono::TouchEvent &)
{
    isPressedDown = true;
    this->scheduleRepaint();
}

void AbstractButtonView::TouchMove(mono::TouchEvent &event)
{
    bool inRect = viewRect.contains(event.Position);

    if (isPressedDown && !inRect) {
        isPressedDown = false;
        scheduleRepaint();
    }
    else if (!isPressedDown && inRect)
    {
        isPressedDown = true;
        scheduleRepaint();
    }
}

void AbstractButtonView::TouchEnd(mono::TouchEvent &event)
{
    bool shouldRepaint = isPressedDown;

    isPressedDown = false;

    // touch is stil within button area,
    if (viewRect.contains(event.Position))
    {
        clickHandler.call();
    }

    if (shouldRepaint)
        scheduleRepaint();
}
