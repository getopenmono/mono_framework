// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "responder_view.h"
#include "consoles.h"

using namespace mono::ui;

ResponderView::ResponderView()
{
    deactivate(); // view starts hidden, do not handle touch
    touchActive = false;
    breakOldLoop = false;
}

ResponderView::ResponderView(geo::Rect rect) : View(rect)
{
    deactivate(); // view starts hidden, do not handle touch
    touchActive = false;
    breakOldLoop = false;
}

void ResponderView::touchBegin(mono::TouchEvent &event)
{
    if (breakOldLoop)
        return;

    breakOldLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    TouchBegin(event);
#pragma GCC diagnostic pop
    breakOldLoop = false;
}

void ResponderView::touchEnd(mono::TouchEvent &event)
{
    if (breakOldLoop)
        return;

    breakOldLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    TouchEnd(event);
#pragma GCC diagnostic pop
    breakOldLoop = false;
}

void ResponderView::touchMove(mono::TouchEvent &event)
{
    if (breakOldLoop)
        return;

    breakOldLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    TouchMove(event);
#pragma GCC diagnostic pop
    breakOldLoop = false;
}

void ResponderView::respondTouchBegin(mono::TouchEvent &event)
{
    if (!event.IsScreenCoords)
    {
        this->toScreenCoords(&event);
    }
    
    if (this->viewRect.contains(event.Position))
    {
        touchActive = true;
        touchBegin(event);
        event.handled = true;
    }
    
}

void ResponderView::respondTouchMove(mono::TouchEvent &event)
{
    
    if (touchActive)
    {
        if (!event.IsScreenCoords)
        {
            this->toScreenCoords(&event);
        }
        
        touchMove(event);
        event.handled = true;
    }
    
}

void ResponderView::respondTouchEnd(mono::TouchEvent &event)
{
    
    if (touchActive == true)
    {
        if (!event.IsScreenCoords) {
            this->toScreenCoords(&event);
        }
        
        touchEnd(event);
        event.handled = true;
        touchActive = false;
    }
}

void ResponderView::toScreenCoords(mono::TouchEvent *event)
{
    int x = event->TouchController->toScreenCoordsX(event->Position.X(), DisplayWidth());
    event->Position.setX(x);
    
    int y = event->TouchController->toScreenCoordsY(event->Position.Y(), DisplayHeight());
    event->Position.setY(y);
    
    event->IsScreenCoords = true;
}

void ResponderView::show()
{
    View::show();
    activate();
}

void ResponderView::hide()
{
    deactivate();
    View::hide();
}
