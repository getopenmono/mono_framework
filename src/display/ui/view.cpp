// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "view.h"
#include <application_context_interface.h>
#include "consoles.h"

#include <us_ticker_api.h>
#include <mbed_debug.h>

using namespace mono::ui;

mono::display::DisplayPainter View::painter(mono::IApplicationContext::Instance->DisplayController);

mono::GenericQueue<View> View::dirtyQueue;

mono::display::Color View::StandardTextColor = display::CloudsColor;
mono::display::Color View::StandardBackgroundColor = display::BlackColor;
mono::display::Color View::StandardBorderColor = display::CloudsColor;
mono::display::Color View::StandardHighlightColor = display::EmeraldColor;

uint32_t View::RepaintScheduledViewsTime = 0;

void View::repaintScheduledViews()
{

    //early exit if queue is empty
    if (View::dirtyQueue.peek() == NULL)
        return;

    uint32_t start = us_ticker_read();
    View *view = View::dirtyQueue.dequeue();
    while (view != NULL) {

        if (view->isDirty)
        {
            view->repaint();
            view->isDirty = false;
#ifdef VIEW_BOUNDARY_DEBUG
            painter.setForegroundColor(mono::display::RedColor);
            painter.drawRect(view->ViewRect());
#endif
        }


        view = View::dirtyQueue.dequeue();
    }
    uint32_t end = us_ticker_read();

    RepaintScheduledViewsTime = end - start;

    //debug("repaint time: %u, TE Offset: %i\r\n",RepaintScheduledViewsTime,start-painter.DisplayController()->LastTearningEffectTime);
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
    dirtyQueue.remove(this);
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
    return viewRect;
}

mono::geo::Size& View::Size()
{
    return viewRect.Size();
}

const mono::geo::Rect& View::ViewRect() const
{
    return viewRect;
}

void View::scheduleRepaint()
{
    if (isDirty || !visible)
        return;

    isDirty = true;
    dirtyQueue.enqueue((View*) this);
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
    isDirty = false;
    dirtyQueue.remove(this);
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
