// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_touch_system.h"
#include "application_context_interface.h"

#include <wait_api.h>
#include <us_ticker_api.h>

#include "consoles.h"

using namespace mono;

MonoTouchSystem::MonoTouchSystem() : CalMinX(370), CalMinY(300), CalMaxX(2980), CalMaxY(3280)
{
    active = true;
}

void MonoTouchSystem::init()
{
    
    touchInProgress = false;
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x00);
    
    ADC_SAR_1_Start();
    
    lastTouchProcess = 0;
}


void MonoTouchSystem::processTouchInput()
{
    if (!active)
        return;
    
    if (TouchResponder::FirstResponder() == 0)
        return;

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
            xAvg.clear(lastLastPosition.X());
            yAvg.clear(lastLastPosition.Y());
        }
    }
    else if (touchInProgress)
    {
        xAvg.append(X); yAvg.append(Y);
        geo::Point newPos = geo::Point(xAvg.value(),yAvg.value());
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
        xAvg.clear(X); yAvg.clear(Y);
        
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
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x00); // no conn.
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
    
    CY_SET_REG8(CYREG_PRT1_AMUX, 0x00); // no conn.
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

TouchCalibration MonoTouchSystem::CurrentCalibration()
{
    return geo::Rect(CalMinX, CalMinY, CalMaxX-CalMinX, CalMaxY - CalMinY);
}

void MonoTouchSystem::setCalibration(TouchCalibration &cal)
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

}

void MonoTouchSystem::onSystemWakeFromSleep()
{

}

void MonoTouchSystem::OnSystemBatteryLow()
{
    
}
