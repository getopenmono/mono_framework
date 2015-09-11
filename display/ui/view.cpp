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

mono::display::DisplayPainter View::painter(mono::IApplicationContext::Instance->displayController);

View *View::firstDirtyView = NULL;

View::View()
{
    nextDirtyView = NULL;
}

void View::setPosition(geo::Point pos)
{
    this->position = pos;
}

void View::setSize(geo::Size siz)
{
    this->size = siz;
}

mono::geo::Point& View::Position()
{
    return position;
}

mono::geo::Size& View::Size()
{
    return size;
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