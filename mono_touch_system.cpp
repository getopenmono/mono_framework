//
//  mono_touch_system.cpp
//  analog
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "mono_touch_system.h"
#include <wait_api.h>
#include "consoles.h"

using namespace mono;

MonoTouchSystem::MonoTouchSystem() : CalMinX(620), CalMinY(490), CalMaxX(2900),CalMaxY(3130)
{
}

void MonoTouchSystem::init()
{
    touchInProgress = false;
    
    CY_SET_REG8(CYREG_PRT6_AMUX, 0x00);
    
    ADC_SAR_1_Start();
}


void MonoTouchSystem::processTouchInput()
{
    int32_t X,Y;
    for (int i=0; i<16; i++) {
        X += sampleX();
        Y += sampleY();
    }
    X = X / 16;
    Y = Y / 16;
    
    
    if (X < CalMinX || Y < CalMinY)
    {
        if (touchInProgress)
        {
            touchInProgress = false;
            runTouchEnd(lastTouchPosition); // use last move position,
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
            lastTouchPosition = newPos;
            runTouchMove(lastTouchPosition);
        }
    }
    else if (!touchInProgress)
    {
        lastTouchPosition = geo::Point(X,Y);
        touchInProgress = true;
        runTouchBegin(lastTouchPosition);
    }
}


uint16_t MonoTouchSystem::sampleX()
{
    CyPins_SetPinDriveMode(TFT_TOUCH_Y1, CY_PINS_DM_ALG_HIZ);
    CyPins_SetPinDriveMode(TFT_TOUCH_Y2, CY_PINS_DM_RES_DWN);
    
    CyPins_SetPinDriveMode(TFT_TOUCH_X1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(TFT_TOUCH_X2, CY_PINS_DM_STRONG);
    
    CyPins_ClearPin(TFT_TOUCH_X1);
    CyPins_SetPin(TFT_TOUCH_X2);
    
    CY_SET_REG8(CYREG_PRT6_AMUX, 0x01);
    
    wait_us(10);
    
    ADC_SAR_1_StartConvert();
    ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
    
    uint16_t sample = ADC_SAR_1_GetResult16();
    
    CyPins_ClearPin(TFT_TOUCH_X2);
    
    return sample;
}

uint16_t MonoTouchSystem::sampleY()
{
    CyPins_SetPinDriveMode(TFT_TOUCH_X1, CY_PINS_DM_ALG_HIZ);
    CyPins_SetPinDriveMode(TFT_TOUCH_X2, CY_PINS_DM_RES_DWN);
    
    
    CyPins_SetPinDriveMode(TFT_TOUCH_Y1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(TFT_TOUCH_Y2, CY_PINS_DM_STRONG);
    
    CyPins_ClearPin(TFT_TOUCH_Y1);
    CyPins_SetPin(TFT_TOUCH_Y2);
    
    CY_SET_REG8(CYREG_PRT6_AMUX, 0x02);
    
    wait_us(10);
    
    ADC_SAR_1_StartConvert();
    ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
    
    uint16_t sample = ADC_SAR_1_GetResult16();
    
    CyPins_ClearPin(TFT_TOUCH_Y2);
    
    return sample;
}

int MonoTouchSystem::ToScreenCoordsX(int touchPos, uint16_t screenWidth)
{
    const uint16_t factor (1000*screenWidth/(CalMaxX-CalMinX));
    return ((touchPos-CalMinX) * factor) >> 10;
}

int MonoTouchSystem::ToScreenCoordsY(int touchPos, uint16_t screenHeight)
{
    const uint16_t factor (1000*screenHeight/(CalMaxY-CalMinY));
    return ((touchPos-CalMinY) * factor) >> 10;
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