//
//  queue_interrupt.cpp
//  mono
//
//  Created by Kristoffer Andersen on 13/10/15.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "queue_interrupt.h"
#include "application_context_interface.h"
#include <us_ticker_api.h>

using namespace mono;

QueueInterrupt::QueueInterrupt(PinName pin, PinMode mode) : mbed::InterruptIn(pin)
{
    this->mode(mode);
    this->addedToRunLoop = false;
    this->fallEvent = this->riseEvent = false;
//    this->changeEvent = false;
}



QueueInterrupt::~QueueInterrupt()
{
    deactivateQueueTaskHandler();
}

void QueueInterrupt::rise(void (*fptr)())
{
    activateQueueTaskHandler();
    _queue_rise.attach(fptr);
    _rise.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_rise_handler);
    gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
}

void QueueInterrupt::fall(void (*fptr)())
{
    activateQueueTaskHandler();
    _queue_fall.attach(fptr);
    _fall.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_fall_handler);
    gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
}

//void QueueInterrupt::change(void (*fptr)())
//{
//    activateQueueTaskHandler();
//    __queue_change.attach(fptr);
//    _fall.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_change_handler);
//    gpio_irq_set(&gpio_irq, (gpio_irq_event)3, 1);
//}


void QueueInterrupt::_irq_rise_handler()
{
    this->riseEvent = true;
    this->riseTimeStamp = us_ticker_read();
}

void QueueInterrupt::_irq_fall_handler()
{
    this->fallEvent = true;
    this->fallTimeStamp = us_ticker_read();
}

//void QueueInterrupt::_irq_change_handler()
//{
//    changeEvent = true;
//    if (CY_GET_REG8(this->gpio_irq.snapShotAddr))
//}

void QueueInterrupt::taskHandler()
{
    if (riseEvent) {
        riseEvent = false;
        _queue_rise.call();
    }
    if (fallEvent)
    {
        fallEvent = false;
        _queue_fall.call();
    }
//    if (changeEvent)
//    {
//        changeEvent = false;
//        _change.call();
//    }
}

uint32_t QueueInterrupt::FallTimeStamp()
{
    return fallTimeStamp;
}

uint32_t QueueInterrupt::RiseTimeStamp()
{
    return riseTimeStamp;
}

bool QueueInterrupt::Snapshot()
{
    return snapShot;
}

void QueueInterrupt::activateQueueTaskHandler()
{
    if (!addedToRunLoop)
    {
        IApplicationContext::Instance->RunLoop->addDynamicTask(this);
        addedToRunLoop = true;
    }
    
}
void QueueInterrupt::deactivateQueueTaskHandler()
{
    if (addedToRunLoop)
    {
        IApplicationContext::Instance->RunLoop->removeDynamicTask(this);
        addedToRunLoop = false;
    }
    
}