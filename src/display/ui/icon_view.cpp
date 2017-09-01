// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "icon_view.h"

using namespace mono::ui;

IconView::IconView() : View()
{
    icon = 0;
}

IconView::IconView(const geo::Point &pos, const display::MonoIcon &icon) :
    View(geo::Rect(pos, geo::Size(icon.width, icon.height)))
{
    this->icon = &icon;
    foreground = View::StandardTextColor;
    background = View::StandardBackgroundColor;
}

// MARK: Accessors

void IconView::setForeground(Color c)
{
    foreground = c;
}

void IconView::setBackground(Color c)
{
    background = c;
}

void IconView::setIcon(const MonoIcon *icon)
{
    this->icon = icon;
}

Color IconView::Foreground() const
{
    return foreground;
}

Color IconView::Background() const
{
    return background;
}

void IconView::repaint()
{
    if (icon == 0)
        return;

    display::IDisplayController *ctrl = painter.DisplayController();
    ctrl->setWindow(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height());

    int cnt = 0;
    for(int y=0; y<icon->height; y++)
    {
        for (int x=0; x<icon->width; x++)
        {
            ctrl->write(foreground.alphaBlend(icon->bitmap[cnt], background));
            cnt++;
        }
    }
}
