
#include "scene_controller.h"

using namespace mono::ui;

SceneController::Scene() : View()
{
    viewRect = geo::Rect(geo::Point(0,0), DisplaySize());
    backColor = StandardBackgroundColor;
}

SceneController::Scene(const geo::Rect &rect) : View(rect)
{
    backColor = StandardBackgroundColor;
}

void SceneController::addView(const View &child)
{
    childviewList.push_back((View*)&child);
}

void SceneController::removeView(const View &child)
{
    childviewList.remove((View*)&child);
}

void SceneController::show()
{
    if (Visible())
        return;
    
    View::show();
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->show();
    }
    
    showHandler.call(*this);
}

void SceneController::hide()
{
    if (!Visible())
        return;
    
    View::hide();
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->hide();
    }
    
    hideHandler.call(*this);
}

void SceneController::requestDismiss()
{
    dismissHandler.call();
}

void SceneController::repaint()
{
    //paint it back
    painter.setBackgroundColor(backColor);
    painter.drawFillRect(viewRect, true);
    
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->scheduleRepaint();
    }
}

mono::display::Color SceneController::BackgroundColor() const
{
    return backColor;
}

void SceneController::setBackground(display::Color color)
{
    backColor = color;
}
