

#include "tabbar_view.h"

using namespace mono::ui;

void TabBarView::init()
{
    btnCount = 0;
    background = StandardBackgroundColor;
    borderColor = display::MidnightBlueColor;
    activeButton = 0;
}

TabBarView::TabBarView()
{
    init();
}

TabBarView::TabBarView(const Point &pos) :
    View(Rect(pos, geo::Size(DisplayWidth(), tabBarHeight)))
{
    init();
}

TabBarView::TabBarView(const Point &pos,
                       IconButtonView &btn1) :
    View(Rect(pos, geo::Size(DisplayWidth(), tabBarHeight)))
{
    init();
    addButton(btn1);
}

TabBarView::TabBarView(const Point &pos,
                       IconButtonView &btn1,
                       IconButtonView &btn2) :
    View(Rect(pos, geo::Size(DisplayWidth(), tabBarHeight)))
{
    init();
    addButton(btn1);
    addButton(btn2);
}

TabBarView::TabBarView(const Point &pos,
                       IconButtonView &btn1,
                       IconButtonView &btn2,
                       IconButtonView &btn3) :
    View(Rect(pos, geo::Size(DisplayWidth(), tabBarHeight)))
{
    init();
    addButton(btn1);
    addButton(btn2);
    addButton(btn3);
}

void TabBarView::buttonPushedHandler(IconButtonView &btn)
{
    setActiveButton(btn);
}

bool TabBarView::setActiveButton(IconButtonView &btn)
{
    bool found = false;
    for(std::list<IconButtonView*>::iterator it = tabButtons.begin(); it != tabButtons.end(); ++it)
    {
        if (*it == &btn)
        {
            found = true;
            (*it)->setIconForeground(display::CloudsColor);
        }
        else
            (*it)->setIconForeground(display::AsbestosColor);
        
        (*it)->scheduleRepaint();
    }
    
    if (found)
    {
        activeButton = &btn;
        btnHandler.call(btn);
        return true;
    }
    else
    {
        activeButton = 0;
        return false;
    }
}

IconButtonView* TabBarView::currentActiveButton() const
{
    return activeButton;
}

// MARK: Accessor Methods

void TabBarView::addButton(IconButtonView &btn)
{
    btn.setIconForeground(display::AsbestosColor);
    btn.setHighlight(display::WhiteColor);
    btn.setDrawsBackground(false);
    btn.setDrawsBorder(false);
    btn.setPushCallback(this, &TabBarView::buttonPushedHandler);
    
    tabButtons.push_back(&btn);
    btnCount++;
    repositionButtonLayout();
}

void TabBarView::removeButton(IconButtonView &btn)
{
    tabButtons.remove(&btn);
    btnCount--;
    repositionButtonLayout();
}

void TabBarView::setButtonPushCallback(void (*cfunc)(IconButtonView &))
{
    btnHandler.attach(cfunc);
}

std::list<IconButtonView*>::const_iterator TabBarView::buttonListBegin() const
{
    return tabButtons.begin();
}

std::list<IconButtonView*>::const_iterator TabBarView::buttonListEnd() const
{
    return tabButtons.end();
}

// MARK: Aux View Methods

void TabBarView::repositionButtonLayout()
{
    int buttonWidth = viewRect.Width() / btnCount;
    Point offset = viewRect.UpperLeft();
    geo::Size size(buttonWidth, tabBarHeight);
    
    for(std::list<IconButtonView*>::iterator it = tabButtons.begin(); it != tabButtons.end(); ++it)
    {
        (*it)->setRect(Rect(offset, size));
        offset.appendX(buttonWidth);
    }
}

void TabBarView::show()
{
    View::show();
    
    for(std::list<IconButtonView*>::iterator it = tabButtons.begin(); it != tabButtons.end(); ++it)
    {
        (*it)->show();
    }
}

void TabBarView::hide()
{
    View::hide();
    
    for(std::list<IconButtonView*>::iterator it = tabButtons.begin(); it != tabButtons.end(); ++it)
    {
        (*it)->hide();
    }
}

void TabBarView::scheduleRepaint()
{
    View::scheduleRepaint();
    
    for(std::list<IconButtonView*>::iterator it = tabButtons.begin(); it != tabButtons.end(); ++it)
    {
        (*it)->scheduleRepaint();
    }
}

void TabBarView::repaint()
{
    painter.setBackgroundColor(background);
    painter.setForegroundColor(borderColor);
    painter.drawFillRect(viewRect, true);
    painter.drawLine(viewRect.UpperLeft(), viewRect.UpperRight());
}

