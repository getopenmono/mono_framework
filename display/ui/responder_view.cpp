//
//  responder_view.cpp
//  mono framework
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "responder_view.h"
#include "consoles.h"

using namespace mono::ui;

ResponderView::ResponderView()
{
    touchActive = false;
}

ResponderView::ResponderView(geo::Rect rect) : View(rect)
{
    touchActive = false;
}

void ResponderView::TouchBegin(mono::TouchEvent &event)
{
}

void ResponderView::TouchEnd(mono::TouchEvent &event)
{
}

void ResponderView::TouchMove(mono::TouchEvent &event)
{
}

void ResponderView::RespondTouchBegin(mono::TouchEvent &event)
{
    if (!event.IsScreenCoords)
    {
        this->ToScreenCoords(&event);
    }
    
    if (this->viewRect.contains(event.Position))
    {
        touchActive = true;
        TouchBegin(event);
        event.handled = true;
    }
    
}

void ResponderView::RespondTouchMove(mono::TouchEvent &event)
{
    
    if (touchActive)
    {
        if (!event.IsScreenCoords)
        {
            this->ToScreenCoords(&event);
        }
        
        TouchMove(event);
        event.handled = true;
    }
    
}

void ResponderView::RespondTouchEnd(mono::TouchEvent &event)
{
    
    if (touchActive == true)
    {
        if (!event.IsScreenCoords) {
            this->ToScreenCoords(&event);
        }
        
        TouchEnd(event);
        event.handled = true;
        touchActive = false;
    }
}

void ResponderView::ToScreenCoords(mono::TouchEvent *event)
{
    int x = event->TouchController->ToScreenCoordsX(event->Position.X(), DisplayWidth());
    event->Position.setX(x);
    
    int y = event->TouchController->ToScreenCoordsX(event->Position.Y(), DisplayHeight());
    event->Position.setY(y);
    
    event->IsScreenCoords = true;
}