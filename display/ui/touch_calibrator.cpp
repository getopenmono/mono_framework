//
//  touch_calibrator.cpp
//  mono framework
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "touch_calibrator.h"
#include "application_context_interface.h"
#include "consoles.h"

#include <wait_api.h>
#include <mbed_debug.h>

using namespace mono::ui;

TouchCalibrateView::TouchCalibrateView() : ResponderView(geo::Rect(0,0,View::DisplayWidth(),View::DisplayHeight())), textLbl("Touch the center of the cubes..."), textHeader("Calibration"), blockColor(display::BlueColor)
{
    textHeader.setRect(viewRect);
    textLbl.setRect(viewRect);
    blockSize = 10;
    calStep = 0;
    
    textHeader.setTextSize(2);
    textHeader.setTextColor(display::WhiteColor);
    textHeader.setAlignment(TextLabelView::ALIGN_CENTER);
    textLbl.setTextColor(display::WhiteColor);
    textLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    
    //align text horizontal center
    uint16_t textHeight = textHeader.TextPixelHeight();
    int hSpacing = viewRect.Height() - textHeight;
    
    if (hSpacing > 0)
    {
        textHeader.Position().setY(viewRect.Y() + hSpacing/2 );
        textHeader.Size().setHeight(viewRect.Height() - hSpacing);
    }
    textLbl.Position().setY(textHeader.Position().Y()+textHeader.TextPixelHeight()+2);
    
    makeFirstResponder();
}

TouchCalibrateView::TouchCalibrateView(geo::Rect rct) : ResponderView(rct), textLbl(rct,"Touch the center of the cubes..."), textHeader("Calibration"), blockColor(display::BlueColor)
{
    textHeader.setRect(viewRect);
    textLbl.setRect(viewRect);
    blockSize = 10;
    calStep = 0;
    
    textHeader.setTextSize(2);
    textHeader.setTextColor(display::WhiteColor);
    textHeader.setAlignment(TextLabelView::ALIGN_CENTER);
    textLbl.setTextColor(display::WhiteColor);
    textLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    
    //align text horizontal center
    uint16_t textHeight = textHeader.TextPixelHeight();
    int hSpacing = viewRect.Height() - textHeight;
    
    if (hSpacing > 0)
    {
        textHeader.Position().setY(viewRect.Y() + hSpacing/2 );
        textHeader.Size().setHeight(viewRect.Height() - hSpacing);
    }
    textLbl.Position().setY(textHeader.Position().Y()+textHeader.TextPixelHeight()+2);
    
    makeFirstResponder();
}

void TouchCalibrateView::repaint()
{
    ITouchSystem::Calibration zeroCal(450,400,0,0);
    textHeader.repaint();
    textLbl.repaint();
    
    painter.setForegroundColor(blockColor);
    switch (calStep) {
        case 0:
            IApplicationContext::Instance->TouchSystem->setCalibration(zeroCal);
            this->painter.drawFillRect(viewRect.X(), viewRect.Y(), blockSize, blockSize);
            break;
        case 1:
            painter.drawFillRect(viewRect.X(), viewRect.Y(), blockSize, blockSize, true);
            painter.drawFillRect(viewRect.X2()-blockSize, viewRect.Y(), blockSize, blockSize);
            break;
        case 2:
            painter.drawFillRect(viewRect.X2()-blockSize, viewRect.Y(), blockSize, blockSize, true);
            painter.drawFillRect(viewRect.X2()-blockSize, viewRect.Y2()-blockSize, blockSize, blockSize);
            break;
        case 3:
            painter.drawFillRect(viewRect.X2()-blockSize, viewRect.Y2()-blockSize, blockSize, blockSize, true);
            painter.drawFillRect(viewRect.X(), viewRect.Y2()-blockSize, blockSize, blockSize);
            break;
        default:
            // paint whole screen
            painter.drawFillRect(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height(), true);
            break;
    }
}

void TouchCalibrateView::RespondTouchBegin(TouchEvent &event)
{
    if (calStep < 4)
    {
        cals[calStep] = event.Position;
        calStep++;
        //scheduleRepaint();
        repaint();
        
        event.handled = true;
    }
    else if (calStep == 4)
    {
        Deactivate();
        calDone();
        return;
    }
    
    wait(1.0);
}

void TouchCalibrateView::makeFirstResponder()
{
    if (FirstResponder != NULL && FirstResponder != this)
    {
        TouchResponder *oldFirst = FirstResponder;
        Deactivate(); // remove myself from responder chain
        FirstResponder = NULL;
        Activate(); // put myself as first responder
        oldFirst->Activate(); // insert old first responder into chain
    }
}

void TouchCalibrateView::calDone()
{
    ITouchSystem::Calibration cal;
    
    int yAvg = (cals[0].Y() + cals[1].Y()) / 2;
    cal.setY(yAvg);
    
    yAvg = (cals[2].Y() + cals[3].Y()) / 2;
    cal.setHeight(yAvg - cal.Y());
    
    int xAvg = (cals[0].X() + cals[3].X()) / 2;
    cal.setX(xAvg);
    
    xAvg = (cals[1].X() + cals[2].X()) /2;
    cal.setWidth(xAvg - cal.X());
    
    if (IApplicationContext::Instance->TouchSystem == NULL)
        debug("Calibrator needs a touch system implementation!");
    
    IApplicationContext::Instance->TouchSystem->setCalibration(cal);
    
    doneCallback.call();
}