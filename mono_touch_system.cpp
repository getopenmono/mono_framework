//
//  mono_touch_system.cpp
//  analog
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "mono_touch_system.h"
#include "application_context_interface.h"

#include <wait_api.h>
#include "consoles.h"

using namespace mono;

MonoTouchSystem::MonoTouchSystem() : CalMinX(620), CalMinY(490), CalMaxX(2900),CalMaxY(3130)
{
}

void MonoTouchSystem::init()
{
    
    touchInProgress = false;
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x00);
    
    ADC_SAR_1_Start();
    
    lastTouchProcess = 0;
    
    IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);
}


void MonoTouchSystem::processTouchInput()
{
    
    if ((us_ticker_read() - lastTouchProcess) <= 10000)
        return;
    
    // connect ADC to right AMUXBUS
    CY_SET_REG8(CYREG_BUS_SW3, 0x01);
    
    uint16_t X = sampleX();
    uint16_t Y = sampleY();
    
    // record time before we handle any touch events
    lastTouchProcess = us_ticker_read();
    
    if (X < CalMinX || Y < CalMinY)
    {
        if (touchInProgress)
        {
            touchInProgress = false;
            runTouchEnd(lastLastPosition); // use last move position,
            // since touch input now is outside screen
        }
    }
    else if (touchInProgress)
    {
        geo::Point newPos = geo::Point(X,Y);
        geo::Point diff = newPos - lastTouchPosition;
        uint32_t sqLength = diff.X()*diff.X() + diff.Y()*diff.Y();
        
        if (sqLength > 2304) //approx 3 pixels move
        {
            lastLastPosition = lastTouchPosition;
            lastTouchPosition = newPos;
            
            // wait to pipeline is full
            if (lastLastPosition != lastTouchPosition)
                runTouchMove(lastLastPosition);
        }
    }
    else if (!touchInProgress)
    {
        wait_ms(5);
        X = sampleX(); Y = sampleY();
        
        lastTouchPosition = geo::Point(X,Y);
        lastLastPosition = lastTouchPosition;
        
        touchInProgress = true;
        runTouchBegin(lastTouchPosition);
    }
    
    //disconnect ADC from right AMUXBUS
    CY_SET_REG8(CYREG_BUS_SW3, 0x00);
}


uint16_t MonoTouchSystem::sampleX()
{
    CyPins_SetPinDriveMode(TFT_TOUCH_Y1, CY_PINS_DM_ALG_HIZ);
    CyPins_SetPinDriveMode(TFT_TOUCH_Y2, CY_PINS_DM_RES_DWN);
    
    CyPins_SetPinDriveMode(TFT_TOUCH_X1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(TFT_TOUCH_X2, CY_PINS_DM_STRONG);
    
    CyPins_ClearPin(TFT_TOUCH_X1);
    CyPins_SetPin(TFT_TOUCH_X2);
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x80); // PC7
    
    uint16_t samples = 0;
    for (int i=0; i<8; i++) {
        wait_us(10);
        
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        
        samples += ADC_SAR_1_GetResult16();
    }
    
    CyPins_ClearPin(TFT_TOUCH_X2);
    
    return samples / 8;
}

uint16_t MonoTouchSystem::sampleY()
{
    CyPins_SetPinDriveMode(TFT_TOUCH_X1, CY_PINS_DM_ALG_HIZ);
    CyPins_SetPinDriveMode(TFT_TOUCH_X2, CY_PINS_DM_RES_DWN);
    
    
    CyPins_SetPinDriveMode(TFT_TOUCH_Y1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(TFT_TOUCH_Y2, CY_PINS_DM_STRONG);
    
    CyPins_ClearPin(TFT_TOUCH_Y1);
    CyPins_SetPin(TFT_TOUCH_Y2);
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x40); // PC4
    
    uint16_t samples = 0;
    for (int i=0; i<8; i++) {
        wait_us(10);
        
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        
        samples += ADC_SAR_1_GetResult16();
    }
    
    CyPins_ClearPin(TFT_TOUCH_Y2);
    
    return samples / 8;
}

int MonoTouchSystem::ToScreenCoordsX(int touchPos, uint16_t screenWidth)
{
    const uint16_t factor = (1024*screenWidth/(CalMaxX-CalMinX));
    return ((touchPos-CalMinX) * factor) >> 10;
}

int MonoTouchSystem::ToScreenCoordsY(int touchPos, uint16_t screenHeight)
{
    const uint16_t factor = (1024*screenHeight/(CalMaxY-CalMinY));
    return ((touchPos-CalMinY) * factor) >> 10;
}

void MonoTouchSystem::setCalibration(ITouchSystem::Calibration &cal)
{
    CalMinX = cal.X();
    CalMaxX = cal.X2();
    CalMinY = cal.Y();
    CalMaxY = cal.Y2();
}

void MonoTouchSystem::onSystemPowerOnReset()
{
    
}

void MonoTouchSystem::onSystemEnterSleep()
{
    ADC_SAR_1_SaveConfig();
    ADC_SAR_1_Sleep();
}

void MonoTouchSystem::onSystemWakeFromSleep()
{
    ADC_SAR_1_Wakeup();
    ADC_SAR_1_RestoreConfig();
}

void MonoTouchSystem::OnSystemBatteryLow()
{
    
}