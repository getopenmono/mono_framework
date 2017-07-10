

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

PowerSaver::PowerSaver(PowerSaver &other)
{
    enabled = other.enabled;
    dimBright = other.dimBright;
    fullBright = other.fullBright;
    stepDir = DIM_NONE;

    dimTimer.setCallback<PowerSaver>(this, &PowerSaver::dim);
    sleepTimer.setCallback<PowerSaver>(this, &PowerSaver::sleepStep);

    other.deactivate();

    startDimTimer();
}

PowerSaver& PowerSaver::operator=(PowerSaver &other)
{
    enabled = other.enabled;
    dimBright = other.dimBright;
    fullBright = other.fullBright;
    stepDir = DIM_NONE;

    dimTimer.setCallback<PowerSaver>(this, &PowerSaver::dim);
    sleepTimer.setCallback<PowerSaver>(this, &PowerSaver::sleepStep);

    other.deactivate();

    startDimTimer();

    return *this;
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
    dimTimer.start();
    sleepTimer.stop();
}
void PowerSaver::startSleepTimer()
{
    enabled = true;
    sleepTimer.start();
    dimTimer.stop();
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
    dimTimer.stop();
    sleepTimer.stop();
    undimStep();
}

void PowerSaver::deactivate()
{
    enabled = false;
    stepDir = DIM_NONE;
    sleepTimer.stop();
    dimTimer.stop();
}

void PowerSaver::RespondTouchBegin(TouchEvent &)
{
    if (enabled)
        undim();
}
