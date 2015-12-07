//
//  view.cpp
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#include "view.h"
#include <application_context_interface.h>
#include "consoles.h"

#include <mbed_debug.h>

using namespace mono::ui;

mono::display::DisplayPainter View::painter(mono::IApplicationContext::Instance->DisplayController);

mono::GenericQueue<View> View::dirtyQueue;

uint32_t View::RepaintScheduledViewsTime = 0;

void View::repaintScheduledViews()
{
    
    //early exit if queue is empty
    if (View::dirtyQueue.Peek() == NULL)
        return;
    
    uint32_t start = us_ticker_read();
    View *view = View::dirtyQueue.Dequeue();
    while (view != NULL) {
        
        if (view->isDirty)
        {
            view->repaint();
            view->isDirty = false;
        }
        
        
        view = View::dirtyQueue.Dequeue();
    }
    uint32_t end = us_ticker_read();
    
    RepaintScheduledViewsTime = end - start;
    
    debug("repaint time: %u\n\r",RepaintScheduledViewsTime);
}


View::View()
{
    isDirty = false;
    visible = false;
    painter.setRefreshCallback<View>(this, &View::callRepaintScheduledViews);
}

View::View(geo::Rect rect) : viewRect(rect)
{
    visible = false;
    isDirty = false;
    painter.setRefreshCallback<View>(this, &View::callRepaintScheduledViews);
}

View::~View()
{
    //remove me from the dirty queue, if I am present there
    dirtyQueue.Remove(this);
}

void View::callRepaintScheduledViews()
{
    View::repaintScheduledViews();
}

void View::setPosition(geo::Point pos)
{
    this->viewRect.setPoint(pos);
}

void View::setSize(geo::Size siz)
{
    this->viewRect.setSize(siz);
}

void View::setRect(geo::Rect rect)
{
    this->viewRect = rect;
}

mono::geo::Point& View::Position()
{
    return viewRect.Point();
}

mono::geo::Size& View::Size()
{
    return viewRect.Size();
}

void View::scheduleRepaint()
{
    if (isDirty || !visible)
        return;
    
    isDirty = true;
    dirtyQueue.Enqueue((View*) this);
}

bool View::Visible() const
{
    return visible;
}

void View::show()
{
    visible = true;
    scheduleRepaint();
}

void View::hide()
{
    visible = false;
    dirtyQueue.Remove(this);
}

//// Static methods

uint16_t View::DisplayWidth()
{
    return View::painter.CanvasWidth();
}

uint16_t View::DisplayHeight()
{
    return View::painter.CanvasHeight();
}

mono::geo::Size View::DisplaySize()
{
    return mono::geo::Size(DisplayWidth(), DisplayHeight());
}

View::Orientation View::DisplayOrientation()
{
    //TODO: Implement display orientation!
    return PORTRAIT;
}