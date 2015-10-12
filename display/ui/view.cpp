//
//  view.cpp
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#include "view.h"
#include <application_context_interface.h>

using namespace mono::ui;

mono::display::DisplayPainter View::painter(mono::IApplicationContext::Instance->DisplayController);

View *View::firstDirtyView = NULL;

View::View()
{
    nextDirtyView = NULL;
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
    if (View::firstDirtyView == NULL)
    {
        // queue is empty, put outselves on top
        View::firstDirtyView = this;
    }
    else
    {
        // find the end of the queue
        View *next = View::firstDirtyView;
        while (next->nextDirtyView != NULL) {
            next = next->nextDirtyView;
        }
        next->nextDirtyView = this;
    }
}

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