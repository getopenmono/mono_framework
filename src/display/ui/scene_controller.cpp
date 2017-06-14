
#include "scene_controller.h"
#include <view.h>

using namespace mono::ui;

SceneController::SceneController() :
    viewRect(geo::Rect(geo::Point(0,0), View::DisplaySize())),
    background(viewRect)
{
    visible = false;
}

SceneController::SceneController(const geo::Rect &rect) :
    viewRect(rect),
    background(rect)
{
    visible = false;
}

// MARK: Scene methods

void SceneController::addView(const IViewALike &child)
{
    childviewList.push_back((IViewALike*)&child);
}

void SceneController::removeView(const IViewALike &child)
{
    childviewList.remove((IViewALike*)&child);
}

void SceneController::requestDismiss()
{
    dismissHandler.call();
}

mono::display::Color SceneController::BackgroundColor() const
{
    return background.Color();
}

void SceneController::setBackground(display::Color color)
{
    background.setBackgroundColor(color);
}

// MARK: ViewALike states

bool  SceneController::Visible() const
{
    return visible;
}

void SceneController::show()
{
    if (Visible())
        return;

    background.show();
    visible = true;

    for(std::list<IViewALike*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->show();
    }

    showHandler.call(*this);
}

void SceneController::hide()
{
    if (!Visible())
        return;

    visible = false;
    for(std::list<IViewALike*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->hide();
    }

    background.hide();
    hideHandler.call(*this);
}

void SceneController::scheduleRepaint()
{
    //paint it black?
    background.scheduleRepaint();

    for(std::list<IViewALike*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->scheduleRepaint();
    }
}

// MARK: ViewALike area & position

void SceneController::setRect(mono::geo::Rect rct)
{
    viewRect = rct;
}

const mono::geo::Rect& SceneController::ViewRect() const
{
    return viewRect;
}

mono::geo::Point& SceneController::Position()
{
    return viewRect;
}

mono::geo::Size& SceneController::Size()
{
    return viewRect;
}
