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
    blockMargin = 15;
    calStep = 0;
    activateTouchOnRepaint = false;
    
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
    blockMargin = 15;
    calStep = 0;
    activateTouchOnRepaint = false;
    
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
    if (activateTouchOnRepaint)
    {
        defaultSerial.printf("making first resonder...\n\r");
        makeFirstResponder();
        activateTouchOnRepaint = false;
        
        if (calStep == 0)
        {
            //begin new calibration, paint screen black first
            painter.drawFillRect(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height());
        }
    }
    
    textHeader.repaint();
    textLbl.repaint();
    
    ITouchSystem::Calibration zeroCal(450,400,0,0);
    
    geo::Rect prevRect(0,0,0,0);
    geo::Rect newRect;
    
    painter.setForegroundColor(blockColor);
    switch (calStep) {
        case 0:
            IApplicationContext::Instance->TouchSystem->setCalibration(zeroCal);
            newRect = geo::Rect(viewRect.X()+blockMargin, viewRect.Y()+blockMargin, blockSize, blockSize);
            drawTouchBlock(newRect, prevRect);
            break;
        case 1:
            prevRect = geo::Rect(viewRect.X()+blockMargin, viewRect.Y()+blockMargin, blockSize, blockSize);
            newRect = geo::Rect(viewRect.X2()-blockSize-blockMargin, viewRect.Y()+blockMargin, blockSize, blockSize);
            drawTouchBlock(newRect, prevRect);
            break;
        case 2:
            prevRect = geo::Rect(viewRect.X2()-blockSize-blockMargin, viewRect.Y()+blockMargin, blockSize, blockSize);
            newRect = geo::Rect(viewRect.X2()-blockSize-blockMargin, viewRect.Y2()-blockSize-blockMargin, blockSize, blockSize);
            drawTouchBlock(newRect, prevRect);
            break;
        case 3:
            prevRect = geo::Rect(viewRect.X2()-blockSize-blockMargin, viewRect.Y2()-blockSize-blockMargin, blockSize, blockSize);
            newRect = geo::Rect(viewRect.X()+blockMargin, viewRect.Y2()-blockSize-blockMargin, blockSize, blockSize);
            drawTouchBlock(newRect, prevRect);
            break;
        default:
            // paint whole screen
            painter.drawFillRect(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height(), true);
            break;
    }
    
}

void TouchCalibrateView::drawTouchBlock(geo::Rect &rect, geo::Rect &prevRect)
{
    painter.setForegroundColor(blockColor);
    painter.drawFillRect(prevRect.X(), prevRect.Y(), prevRect.Width(), prevRect.Height(), true);
    painter.drawFillRect(rect.X(), rect.Y(), rect.Width(), rect.Height());
    
    painter.setForegroundColor(display::WhiteColor);
    painter.drawPixel(rect.X()+rect.Width()/2, rect.Y()+rect.Height()/2);
}

void TouchCalibrateView::RespondTouchBegin(TouchEvent &event)
{
    if (event.IsScreenCoords)
    {
        textHeader.setText("Error");
        textLbl.setText("Calibration must be FirstResponder!");
        textHeader.scheduleRepaint();
        textLbl.scheduleRepaint();
        return;
    }
    
    touchActive = true;
    event.handled = true;
    
    if (calStep < 4)
    {
        cals[calStep] = event.Position;
        calStep++;
        
        //deactive touch input, since we must allow the user to lift her finger
        // from the screen. (Touch de-bouncing)
        Deactivate();
        activateTouchOnRepaint = true;
        scheduleRepaint();
    }
    
    if (calStep == 4)
    {
        defaultSerial.printf("calibration done!\n\r");
        activateTouchOnRepaint = false;
        Deactivate();
        calDone();
        return;
    }
}

void TouchCalibrateView::makeFirstResponder()
{
    if (FirstResponder() != this)
    {
        TouchResponder *oldFirst = FirstResponder();
        Deactivate(); // remove myself from responder chain
        ResponderChain.Dequeue(); // removes first responder!
        Activate(); // put myself as first responder
        
        if (oldFirst != NULL)
            oldFirst->Activate(); // insert old first responder into chain
    }
}

void TouchCalibrateView::StartNewCalibration()
{
    calStep = 0;
    activateTouchOnRepaint = true;
    
    scheduleRepaint();
}

void TouchCalibrateView::calDone()
{
    ITouchSystem::Calibration cal;
    
    int yAvg1 = (cals[0].Y() + cals[1].Y()) / 2;
    int yAvg2 = (cals[2].Y() + cals[3].Y()) / 2;
    
    int deltaYpx = viewRect.Height() - 2*blockMargin - blockSize; // blockSize = 2 * blockSize/2;
    int deltaYt  = yAvg2 - yAvg1;
    
    int milliScale = (1024*deltaYt)/deltaYpx;
    
    // set minimum touch value for Y axis
    int PXoffset = blockMargin + blockSize/2;
    cal.setY( yAvg1 - (milliScale*PXoffset)/1024 );
    
    // set maximum touch value for Y axis
    cal.setHeight( yAvg2 + (milliScale*PXoffset)/1024 - cal.Y());
    
    
    int xAvg1 = (cals[0].X() + cals[3].X()) / 2;
    int xAvg2 = (cals[1].X() + cals[2].X()) /2;
    
    int deltaXpx = viewRect.Width() - 2*blockMargin - blockSize;
    int deltaXt  = xAvg2 - xAvg1;
    milliScale = (1024*deltaXt)/deltaXpx;
    
    cal.setX(xAvg1 - (milliScale*PXoffset)/1024);
    
    cal.setWidth( xAvg2 + (milliScale*PXoffset)/1024 - cal.X());
    
    if (IApplicationContext::Instance->TouchSystem == NULL)
        debug("Calibrator needs a touch system implementation!");
    
    debug("Calibration is: (%i, %i, %i, %i)\n\r", cal.X(), cal.Y(), cal.X2(), cal.Y2());
    IApplicationContext::Instance->TouchSystem->setCalibration(cal);
    
    doneCallback.call();
}