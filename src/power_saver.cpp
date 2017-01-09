

#include "power_saver.h"
using namespace mono;


PowerSaver::PowerSaver(int dimTimeoutMs, int sleepTimeoutMs,
                       int dimBrightness, int fullBrightness) :
    dimTimer(dimTimeoutMs, true),
    sleepTimer(sleepTimeoutMs, true)
{
    dimBright = dimBrightness;
    fullBright = fullBrightness;
    
    dimTimer.setCallback<PowerSaver>(this, &PowerSaver::dim);
    sleepTimer.setCallback(&IApplicationContext::EnterSleepMode);
    
    startDimTimer();
}

void PowerSaver::dimStep()
{
    int cur = IApplicationContext::Instance->DisplayController->Brightness();
    if (cur <= dimBright)
        startSleepTimer();
    else
    {
        IApplicationContext::Instance->DisplayController->setBrightness(cur-1);
        async<PowerSaver>(this, &PowerSaver::dimStep);
    }
}

void PowerSaver::startDimTimer()
{
    dimTimer.Start();
    sleepTimer.Stop();
}
void PowerSaver::startSleepTimer()
{
    sleepTimer.Start();
    dimTimer.Stop();
}

void PowerSaver::dim()
{
    dimStep();
}

void PowerSaver::undim()
{
    dimTimer.Stop();
    sleepTimer.Stop();
    IApplicationContext::Instance->DisplayController->setBrightness(fullBright);
    dimTimer.Start();
}

void PowerSaver::deactivate()
{
    sleepTimer.Stop();
    dimTimer.Stop();
}

void PowerSaver::RespondTouchBegin(TouchEvent &)
{
    undim();
}
