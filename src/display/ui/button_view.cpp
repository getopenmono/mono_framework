// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include <deprecated.h>
#include "button_view.h"
#include "consoles.h"

using namespace mono::ui;

// MARK: Constructors

void ButtonView::initButton()
{
    textLabel.setAlignment(TextLabelView::ALIGN_CENTER);

    //align text horizontal center
    geo::Size dimen = textLabel.TextDimension();
    int hSpacing = viewRect.Height() - dimen.Height();

    if (hSpacing > 0)
    {
        textLabel.Position().setY(viewRect.Y() + hSpacing/2 );
        textLabel.Size().setHeight(viewRect.Height() - hSpacing);
    }

    isPressedDown = false;
    textColor = StandardTextColor;
    borderColor = StandardBorderColor;
    borderColorPressed = StandardHighlightColor;
}

ButtonView::ButtonView() :
    ResponderView(),
    textLabel(geo::Rect(), "")
{
    initButton();
}

ButtonView::ButtonView(geo::Rect rect, String text) :
    ResponderView(rect),
    textLabel(geo::Rect(rect), text)
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

// MARK: Touch Handlers

void ButtonView::touchBegin(mono::TouchEvent &)
{
    isPressedDown = true;
    this->scheduleRepaint();
}

void ButtonView::touchMove(mono::TouchEvent &event)
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

void ButtonView::touchEnd(mono::TouchEvent &event)
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

// MARK: Accessors

void ButtonView::setText(mono::String txt)
{
    textLabel.setText(txt);
}

void ButtonView::setFont(MonoFont const &newFont)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    textLabel.setFont(newFont);
#pragma GCC diagnostic pop
}

void ButtonView::setFont(const GFXfont &newFont)
{
    textLabel.setFont(newFont);
}

void ButtonView::setRect(geo::Rect r)
{
    View::setRect(r);
    textLabel.setRect(r);
}

// colors
void ButtonView::setBorder(Color c)
{
    borderColor = c;
}

void ButtonView::setText(Color c)
{
    textColor = c;
}

void ButtonView::setBackground(Color c)
{
    background = c;
    textLabel.setBackground(c);
}

void ButtonView::setHighlight(Color c)
{
    borderColorPressed = c;
}

const TextLabelView& ButtonView::TextLabel() const
{
    return textLabel;
}

/// Painters

void ButtonView::repaint()
{
    textLabel.incrementalRepaint = false;
    textLabel.setText( isPressedDown ? borderColorPressed : textColor );
    textLabel.repaint();

    painter.setForegroundColor( isPressedDown ? borderColorPressed : borderColor );
    painter.drawRect(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height());

}
