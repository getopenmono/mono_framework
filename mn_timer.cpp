//
//  mn_timer.cpp
//  mono framework
//
//  Created by Kristoffer Andersen on 16/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "mn_timer.h"
#include "application_context_interface.h"
#include "consoles.h"

using namespace mono;


Timer::Timer(uint32_t intervalMs, bool snglShot)
{
    singleShot = false; // this is singleShot property for task loop
    
    running = false;
    timerSingleShot = snglShot;
    interruptDidFire = false;
    interval = intervalMs;
    autoRelease = false;
}

Timer::Timer(const Timer &timer) {}

Timer& Timer::operator=(const mono::Timer &timer) { return *this; }

void Timer::Start()
{
    if (handler == false)
        return;
    
    running = true;
    IApplicationContext::Instance->RunLoop->addDynamicTask(this);
    
    // tockers do not support zero interval
    if (interval > 0)
        ticker.attach_us<Timer>(this, &Timer::hwTimerInterrupt, interval*1000);
    else
        interruptDidFire = true;
}

void Timer::Stop()
{
    running = false;
    IApplicationContext::Instance->RunLoop->removeDynamicTask(this);
    ticker.detach();
}

bool Timer::Running()
{
    return running;
}

bool Timer::SingleShot()
{
    return timerSingleShot;
}

void Timer::setInterval(uint32_t newIntervalMs)
{
    if (running)
    {
        Stop();
        interval = newIntervalMs;
        Start();
    }
    else
        interval = newIntervalMs;
}

void Timer::taskHandler()
{
    bool die = false;
    if (interruptDidFire && running)
    {
        if (timerSingleShot)
        {
            Stop();
            die = true;
        }
        
        interruptDidFire = false;
        
        handler.call();
    }
    
    //should this timer commit suicide?
    if (die && autoRelease)
    {
        delete this;
    }
}

void Timer::hwTimerInterrupt()
{
    interruptDidFire = true;
}

Timer::~Timer()
{
    //CyGlobalIntDisable;
    if (running)
    {
        Stop();
    }
    //CyGlobalIntEnable;
}
