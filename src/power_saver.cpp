

#include "power_saver.h"
using namespace mono;


PowerSaver::PowerSaver(int dimTimeoutMs, int sleepTimeoutMs,
                       int dimBrightness, int fullBrightness) :
    dimTimer(dimTimeoutMs, true),
    sleepTimer(sleepTimeoutMs, true)
{
    enabled = true;
    dimBright = dimBrightness;
    fullBright = fullBrightness;
    stepDir = DIM_NONE;
    
    dimTimer.setCallback<PowerSaver>(this, &PowerSaver::dim);
    sleepTimer.setCallback<PowerSaver>(this, &PowerSaver::sleepStep);
    
    startDimTimer();
}

void PowerSaver::sleepStep()
{
    if (!enabled || stepDir != DIM_DOWN)
        return;

    int cur = IApplicationContext::Instance->DisplayController->Brightness();
    if (cur <= 0)
        IApplicationContext::EnterSleepMode();
    else
    {
        IApplicationContext::Instance->DisplayController->setBrightness(cur-1);
        async<PowerSaver>(this, &PowerSaver::sleepStep);
    }
}

void PowerSaver::dimStep()
{
    if (!enabled || stepDir != DIM_DOWN)
        return;

    int cur = IApplicationContext::Instance->DisplayController->Brightness();
    if (cur <= dimBright)
        startSleepTimer();
    else
    {
        IApplicationContext::Instance->DisplayController->setBrightness(cur-1);
        async<PowerSaver>(this, &PowerSaver::dimStep);
    }
}

void PowerSaver::undimStep()
{
    if (!enabled || stepDir != DIM_UP)
        return;

    int cur = IApplicationContext::Instance->DisplayController->Brightness();
    if (cur >= fullBright)
        startDimTimer();
    else
    {
        IApplicationContext::Instance->DisplayController->setBrightness(cur+1);
        async<PowerSaver>(this, &PowerSaver::undimStep);
    }
}

void PowerSaver::startDimTimer()
{
    enabled = true;
    dimTimer.Start();
    sleepTimer.Stop();
}
void PowerSaver::startSleepTimer()
{
    enabled = true;
    sleepTimer.Start();
    dimTimer.Stop();
}

void PowerSaver::dim()
{
    enabled = true;
    stepDir = DIM_DOWN;
    dimStep();
}

void PowerSaver::undim()
{
    enabled = true;
    stepDir = DIM_UP;
    dimTimer.Stop();
    sleepTimer.Stop();
    undimStep();
}

void PowerSaver::deactivate()
{
    enabled = false;
    stepDir = DIM_NONE;
    sleepTimer.Stop();
    dimTimer.Stop();
}

void PowerSaver::RespondTouchBegin(TouchEvent &)
{
    if (enabled)
        undim();
}
