// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "mn_timer.h"
#include "application_context_interface.h"

using namespace mono;

// MARK: Public Constructors

Timer::Timer()
{
    singleShot = false; // the is the proterty of the run loop task
    running = false;
    timerSingleShot = false;
    interruptDidFire = false;
    interval = 0;
    autoRelease = false;
}

Timer::Timer(uint32_t intervalMs, bool snglShot)
{
    singleShot = false; // this is singleShot property for task loop
    
    running = false;
    timerSingleShot = snglShot;
    interruptDidFire = false;
    interval = intervalMs;
    autoRelease = false;
}

Timer::Timer(const Timer &other)
{
    singleShot = false; // this is singleShot property for task loop
    this->timerSingleShot = other.timerSingleShot;
    this->interval = other.interval;
    this->running = false;
    this->interruptDidFire = false;
    this->autoRelease = other.autoRelease;
    this->handler = other.handler;
}

Timer& Timer::operator=(const mono::Timer &) { return *this; }

// MARK: Public Methods

void Timer::start()
{
    if (handler == false)
        return;

    running = true;
    IApplicationContext::Instance->RunLoop->addDynamicTask(this);

    // tickers do not support zero interval
    if (interval > 0)
        ticker.attach_us<Timer>(this, &Timer::hwTimerInterrupt, interval*1000);
    else
        interruptDidFire = true;
}

void Timer::Start() {
    start();
}

void Timer::stop()
{
    running = false;
    IApplicationContext::Instance->RunLoop->removeDynamicTask(this);
    ticker.detach();
}

void Timer::Stop() {
    stop();
}

bool Timer::Running() const
{
    return running;
}

bool Timer::SingleShot() const
{
    return timerSingleShot;
}

void Timer::setInterval(uint32_t newIntervalMs)
{
    if (running)
    {
        stop();
        interval = newIntervalMs;
        start();
    }
    else
        interval = newIntervalMs;
}

// MARK: Internal Methods

void Timer::taskHandler()
{
    bool die = false;
    if (interruptDidFire && running)
    {
        if (timerSingleShot)
        {
            stop();
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
        stop();
    }
    //CyGlobalIntEnable;
}
