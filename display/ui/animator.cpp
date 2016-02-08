// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "animator.h"
#include "application_context_interface.h"

using namespace mono::ui;


Animator::Animator(View *view)
{
    this->active = false;
    this->view = view;
    origin = view->Position();
    destination = view->Position();
    
    //moveVector.setX(dest.X() - origin.X());
    //moveVector.setY(dest.Y() - origin.Y());
    
}

void Animator::setMoveVector(geo::Point vec)
{
    moveVector = vec;
}

void Animator::setDestination(geo::Point dest)
{
    destination = dest;
}

void Animator::taskHandler()
{
    if (!active)
        return;
    
    int newX = view->viewRect.X()+moveVector.X();
    int newY = view->viewRect.Y()+moveVector.Y();
    
    if (view->viewRect == destination)
    {
        Pause();
        return;
    }
    
    if (newX+view->viewRect.Width() >= View::painter.CanvasWidth())
    {
        newX = View::painter.CanvasWidth() - view->viewRect.Width();
    }
    
    if (newY+view->viewRect.Height() >= View::painter.CanvasHeight())
    {
        newY = View::painter.CanvasHeight() - view->viewRect.Height();
    }
    
    if (newX < 0)
    {
        newX = 0;
    }
    
    if (newY < 0)
    {
        newY = 0;
    }
    
    view->viewRect.setX(newX);
    view->viewRect.setY(newY);
    
    view->repaint();
    //view->scheduleRepaint();
}

void Animator::Start()
{
    active = true;
    IApplicationContext::Instance->RunLoop->addDynamicTask(this);
}

void Animator::Pause()
{
    active = false;
    IApplicationContext::Instance->RunLoop->removeDynamicTask(this);
}

void Animator::Reset()
{
    view->setPosition(origin);
    view->repaint();
    //view->scheduleRepaint();
}