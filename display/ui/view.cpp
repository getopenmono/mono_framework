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

using namespace mono::ui;

//mono::ui::View mono::ui::View::firstDirtyView = NULL;
mono::display::DisplayPainter View::painter(mono::IApplicationContext::Instance->DisplayController);

View *View::firstDirtyView = NULL;

void View::repaintScheduledViews()
{
    if (View::firstDirtyView != NULL)
    {
        View *view = View::firstDirtyView;
        
        while (view != NULL) {
            
            if (view->isDirty)
            {
                view->repaint();
                view->isDirty = false;
            }
            
            view = view->nextDirtyView;
        }
    }
}


View::View()
{
//    if (View::firstDirtyView == NULL)
//    {
//        View::firstDirtyView = this;
//    }
//    else
//    {
//        //insert ourselves in the re-paint queue
//        View *v = View::firstDirtyView;
//        
//        while (v->nextDirtyView != NULL) {
//            v = v->nextDirtyView;
//        }
//        
//        v->nextDirtyView = this;
//    }
    
    isDirty = false;
    nextDirtyView = NULL;
}

View::View(geo::Rect rect) : viewRect(rect)
{
    
}

View::~View()
{
    //remove ourselves from the queue
//    if (View::firstDirtyView == NULL)
//        return;
//    
//    View *item = firstDirtyView;
//    View *previous = NULL;
//    
//    while (item != NULL)
//    {
//        if (item == this)
//        {
//            if (previous != NULL)
//                previous->nextDirtyView = item->nextDirtyView;
//            
//            return;
//        }
//        
//        previous = item;
//        item = item->nextDirtyView;
//    }
    
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
    isDirty = true;
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