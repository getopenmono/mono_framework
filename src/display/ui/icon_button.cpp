
#include "icon_button.h"

using namespace mono::ui;

void IconButtonView::init()
{
    borderColor = StandardBorderColor;
    iconColor = StandardBorderColor;
    borderColorPressed = StandardHighlightColor;
    background = StandardBackgroundColor;
    
    iconView.setForeground(iconColor);
    iconView.setBackground(background);
    
    drawsBorder = true;
    drawsBackground = true;
    isPressedDown = false;
    
    setClickCallback<IconButtonView>(this, &IconButtonView::callPushHandler);
}

IconButtonView::IconButtonView() : AbstractButtonView(),_icon(0)
{
    init();
}

IconButtonView::IconButtonView(const Rect &rct, const MonoIcon &icon) :
    AbstractButtonView(rct),
    iconView(Rect(rct.X()+iconInset, rct.Y()+iconInset, rct.Width()-iconInset*2, rct.Height()-iconInset*2), icon),
    _icon(&icon)
{
    init();
}

void IconButtonView::callPushHandler()
{
    cnxtPushHandler.call(*this);
}

// MARK: Accessor methods

void IconButtonView::setBorder(Color c)
{
    borderColor = c;
}

void IconButtonView::setIconForeground(Color c)
{
    iconColor = c;
    iconView.setForeground(c);
}

void IconButtonView::setHighlight(Color c)
{
    borderColorPressed = c;
}

void IconButtonView::setBackground(Color c)
{
    background = c;
    iconView.setBackground(c);
}

void IconButtonView::setIcon(const MonoIcon &icn)
{
    _icon = &icn;
    iconView.setIcon(&icn);
    scheduleRepaint();
}

void IconButtonView::setRect(geo::Rect rct)
{
    View::setRect(rct);
    repositionIconView();
}

void IconButtonView::setDrawsBorder(bool draws)
{
    drawsBorder = draws;
}

void IconButtonView::setDrawsBackground(bool draws)
{
    drawsBackground = draws;
}

void IconButtonView::setPushCallback(void(*cfunc)(IconButtonView&))
{
    cnxtPushHandler.attach(cfunc);
}

// MARK: Aux view methods

void IconButtonView::repositionIconView()
{
    int xInset = viewRect.Width() / 2 - _icon->width / 2;
    int yInset = viewRect.Height() / 2 - _icon->height / 2;
    iconView.setPosition(geo::Point(viewRect.X()+xInset, viewRect.Y()+yInset));
}

void IconButtonView::repaint()
{
    //draw button border
    painter.setBackgroundColor(background);
    painter.setForegroundColor(isPressedDown ? borderColorPressed : borderColor);
    
    if (drawsBackground)
        painter.drawFillRect(viewRect, true);
    
    if (drawsBorder)
        painter.drawRect(viewRect);
    
    if (isPressedDown)
        iconView.setForeground(borderColorPressed);
    else
        iconView.setForeground(iconColor);
    
    iconView.repaint();
}
