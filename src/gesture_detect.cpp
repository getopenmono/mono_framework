#include "gesture_detect.h"

GestureDetect::GestureDetect()
{
    gestureInProgress = false;
    
}

void GestureDetect::RespondTouchBegin(mono::TouchEvent &event)
{
    if (event.handled)
        return;
    mono::TouchCalibration cali = mono::IApplicationContext::Instance->TouchSystem->CurrentCalibration();

    int maxX = event.IsScreenCoords ? mono::ui::View::DisplayWidth() : cali.Width();
    beginPos = event.Position;
    
    if (event.Position.X() > maxX/2) {
        Begin = RIGHT;
    }
    else {
        Begin = LEFT;
    }
}


void GestureDetect::RespondTouchEnd(mono::TouchEvent &event)
{
    if (event.handled)
        return;
    
    mono::TouchCalibration cali = mono::IApplicationContext::Instance->TouchSystem->CurrentCalibration();
    int maxX = event.IsScreenCoords ? mono::ui::View::DisplayWidth() : cali.Width();
    endPos = event.Position;
    
    if (event.Position.X() > maxX/2) {
        End = RIGHT;
    }
    else {
        End = LEFT;
    }
    
    processMove(event.IsScreenCoords);
}

void GestureDetect::processMove(bool isScreenCoords)
{
    mono::geo::Point diff = endPos - beginPos;
    mono::TouchCalibration cali = mono::IApplicationContext::Instance->TouchSystem->CurrentCalibration();
    
    int maxVal = isScreenCoords ? mono::ui::View::DisplayWidth() : cali.Width();
    if (diff.Abs() < (uint32_t)(maxVal / 2))
    {
        return;
    }
    
    if (Begin == LEFT && End == RIGHT)
        slideLeft.call();
    else if (Begin == RIGHT && End == LEFT)
        slideRight.call();
}


