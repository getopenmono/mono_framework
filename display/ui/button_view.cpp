//
//  button_view.cpp
//  mono framework
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "button_view.h"
#include "consoles.h"

using namespace mono::ui;

void ButtonView::initButton()
{
    textLabel.setAlignment(TextLabelView::ALIGN_CENTER);
    
    //align text horizontal center
    uint16_t textHeight = textLabel.TextPixelHeight();
    int hSpacing = viewRect.Height() - textHeight;
    
    if (hSpacing > 0)
    {
        textLabel.Position().setY(viewRect.Y() + hSpacing/2 );
        textLabel.Size().setHeight(viewRect.Height() - hSpacing);
    }
    
    isPressedDown = false;
}

ButtonView::ButtonView() :
    ResponderView(),
borderColor(View::StandardBorderColor),
    borderColorPressed(StandardHighlightColor)
{
    initButton();
}

ButtonView::ButtonView(geo::Rect rect, String text) :
    ResponderView(rect),
    textLabel(geo::Rect(rect), text),
    borderColor(StandardBorderColor),
    borderColorPressed(StandardHighlightColor)
{
    initButton();
}

ButtonView::ButtonView(geo::Rect rect, const char *text) :
ResponderView(rect),
textLabel(geo::Rect(rect), String(text)),
borderColor(StandardBorderColor),
borderColorPressed(StandardHighlightColor)
{
    initButton();
}

void ButtonView::TouchBegin(mono::TouchEvent &event)
{
    isPressedDown = true;
    this->scheduleRepaint();
}

void ButtonView::TouchMove(mono::TouchEvent &event)
{
    bool inRect = viewRect.contains(event.Position);
    
    if (isPressedDown && !inRect) {
        isPressedDown = false;
        scheduleRepaint();
    }
    else if (!isPressedDown && inRect)
    {
        isPressedDown = true;
        scheduleRepaint();
    }
}

void ButtonView::TouchEnd(mono::TouchEvent &event)
{
    bool shouldRepaint = isPressedDown;
    
    isPressedDown = false;
    
    // touch is stil within button area,
    if (viewRect.contains(event.Position))
    {
        clickHandler.call();
    }
    
    if (shouldRepaint)
        scheduleRepaint();
}

void ButtonView::setText(mono::String txt)
{
    textLabel.setText(txt);
    textLabel.scheduleRepaint();
}

void ButtonView::repaint()
{
    
    textLabel.setTextColor( isPressedDown ? borderColorPressed : borderColor );
    textLabel.repaint();
    
    painter.setForegroundColor( isPressedDown ? borderColorPressed : borderColor );
    painter.drawRect(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height());
    
}