// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "on_off_button_view.h"


using namespace mono::ui;
using namespace mono::geo;
using namespace mono::display;

OnOffButtonView::OnOffButtonView()
{
    isPressedDown = false;
    state = false;
    enabledColor = display::BelizeHoleColor;
    borderColor = StandardBorderColor;
    highlightColor = StandardHighlightColor;
    backgroundColor = StandardBackgroundColor;
}

OnOffButtonView::OnOffButtonView(Rect rct) :
    AbstractButtonView(rct),
    onLbl(rct, "on"),
    offLbl(rct, "off")
{
    isPressedDown = false;
    state = false;
    enabledColor = display::BelizeHoleColor;
    borderColor = StandardBorderColor;
    highlightColor = StandardHighlightColor;
    backgroundColor = StandardBackgroundColor;

    buttonSeparation = 4;
    uint32_t subWidth = (rct.Width()-buttonSeparation)/2;

    onLbl.setRect(geo::Rect(rct.X(), rct.Y(), subWidth, rct.Height()));
    onLbl.setAlignment(TextLabelView::ALIGN_CENTER);

    offLbl.setRect(geo::Rect(rct.X()+buttonSeparation+subWidth, rct.Y(), subWidth, rct.Height()));
    offLbl.setAlignment(TextLabelView::ALIGN_CENTER);
}

// MARK: Setters

void OnOffButtonView::setBackground(Color c)
{
    backgroundColor = c;
    onLbl.setBackground(c);
    offLbl.setBackground(c);
}

void OnOffButtonView::setBorder(display::Color c)
{
    borderColor = c;
    onLbl.setText(c);
    offLbl.setText(c);
}


void OnOffButtonView::setHighlight(display::Color c)
{
    highlightColor = c;
}

// MARK: Getters


Color OnOffButtonView::BackgroundColor() const
{
    return backgroundColor;
}

Color OnOffButtonView::HighlightColor() const
{
    return highlightColor;
}

Color OnOffButtonView::BorderColor() const
{
    return borderColor;
}

bool OnOffButtonView::isActive() const
{
    return state;
}

// MARK: Touch

void OnOffButtonView::TouchEnd(mono::TouchEvent &event)
{
    bool shouldRepaint = isPressedDown;

    isPressedDown = false;

    // touch is stil within button area,
    if (viewRect.contains(event.Position))
    {
        state = !state;
        clickHandler.call();
    }

    if (shouldRepaint)
        scheduleRepaint();
}


void OnOffButtonView::setState(bool on, bool fireClick)
{
    if (state != on)
    {
        state = on;
        if (fireClick)
            clickHandler.call();
        
        scheduleRepaint();
    }
}

// MARK: Paints

void OnOffButtonView::repaint()
{
    geo::Rect onBorder = ViewRect();
    geo::Rect offBorder = ViewRect();

    uint32_t subWidth = (viewRect.Width()-buttonSeparation) / 2;
    onBorder.setWidth(subWidth);
    offBorder.setWidth(subWidth);
    offBorder.setX( viewRect.X()+buttonSeparation+subWidth );

    if (!isPressedDown && state)
    {
        display::Color faded = backgroundColor.alphaBlend(192, display::EmeraldColor);
        onLbl.setBackground( faded );
        painter.setBackgroundColor(faded);
        painter.drawFillRect(onBorder, true);
    }
    else if (!isPressedDown)
    {
        onLbl.setBackground(backgroundColor);
        painter.setBackgroundColor(backgroundColor);
        painter.drawFillRect(onBorder, true);
    }


    //    if (state && !isPressedDown)
    //    {
    ////
    //        onLbl.setTextColor(faded);
    //    }
    //    else
    onLbl.setText( isPressedDown ? highlightColor : borderColor );

    offLbl.setText( isPressedDown ? highlightColor : borderColor );



    onLbl.repaint();
    offLbl.repaint();

    painter.setForegroundColor( isPressedDown ? highlightColor : borderColor );
    painter.drawRect(onBorder);
    painter.setForegroundColor( isPressedDown ? highlightColor : borderColor );
    painter.drawRect(offBorder);

    geo::Rect fatOffBorder(offBorder.X()+1, offBorder.Y()+1, offBorder.Width()-2, offBorder.Height()-2);
    painter.drawRect(fatOffBorder, (state || isPressedDown));

    geo::Rect fatOnBorder(onBorder.X()+1, onBorder.Y()+1, onBorder.Width()-2, onBorder.Height()-2);
    painter.drawRect(fatOnBorder, (!state || isPressedDown));

    //    geo::Point off1(offBorder.UpperLeft()), off2(offBorder.UpperRight());
    //    off1.appendY(1); off1.appendX(1); off2.appendY(1);
    //    painter.drawLine(off1, off2, (state || isPressedDown));
    //
    //    geo::Point on1(onBorder.UpperLeft()), on2(onBorder.UpperRight());
    //    on1.appendY(1); on1.appendX(1); on2.appendY(1);
    //    painter.drawLine(on1, on2, (!state || isPressedDown));
}
