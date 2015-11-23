//
//  application_run_loop.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//
//

#include "application_run_loop.h"
#include "application_context_interface.h"
#include <mbed.h>
#include <consoles.h>

using namespace mono;

AppRunLoop::AppRunLoop()
{
    runLoopActive = true;
    lastDtrValue = true;
    resetOnDTR = true;
    resetOnUserButton = false;
    taskQueueHead = NULL;
}

void AppRunLoop::exec()
{
    debug("mono enter run loop!\n\r");
    
    if (mono::defaultSerial.IsReady())
        lastDtrValue = mono::defaultSerial.DTR();
    else
        lastDtrValue = false;
    
    while (runLoopActive) {
        
        if (mono::defaultSerial.IsReady())
        {
            
            bool dtr = mono::defaultSerial.DTR();
            if (resetOnDTR && (!dtr) && lastDtrValue)
            {
                debug("mono DTR reboot!\n\r");
                wait_ms(50);
                IApplicationContext::SoftwareReset();
            }
            lastDtrValue = dtr;
        }
        
        if (resetOnUserButton)
        {
            //TODO: remove cypress reference here!
            if (CyPins_ReadPin(SW_USER) == 0)
            {
                debug("Will reset on user button!\n\r");
                wait_ms(100);
                IApplicationContext::SoftwareReset();
            }
        }
        
        uint32_t start = us_ticker_read();
        //handle touch inputs
        if (IApplicationContext::Instance->TouchSystem != NULL)
            IApplicationContext::Instance->TouchSystem->processTouchInput();
        uint32_t tEnd = us_ticker_read();
        
        //run scheduled tasks
        processDynamicTaskQueue();
        
        uint32_t end = us_ticker_read();
        
        TouchSystemTime = tEnd - start;
        DynamicTaskQueueTime = end - tEnd;
    }
    
    debug("run loop terminated!");
}


void AppRunLoop::processDynamicTaskQueue()
{
    if (taskQueueHead == NULL)
    {
        return;
    }
    
    
    IRunLoopTask *task = taskQueueHead;
    while (task != NULL) {
        task->taskHandler();
        
        if (task->singleShot)
        {
            debug("Removing task from dynamic queue!\n\r");
            removeTaskInQueue(task);
        }
        
        
        // we can still use the tasks next pointer,
        //  even if its not in the list anymore
        task = task->nextTask;
    }
}


bool AppRunLoop::addDynamicTask(mono::IRunLoopTask *task)
{
    if (taskQueueHead == NULL)
    {
        taskQueueHead = task;
        task->nextTask = NULL;
        task->previousTask = NULL;
    }
    else
    {
        IRunLoopTask *item = taskQueueHead;
        while (item->nextTask != NULL)
        {
            item = item->nextTask;
        }
        
        item->nextTask = task;
        task->previousTask = item;
        task->nextTask = NULL;
    }
    
    return true;
}

bool AppRunLoop::removeDynamicTask(mono::IRunLoopTask *task)
{
    if (taskQueueHead == NULL)
        return false;
    
    IRunLoopTask *item = taskQueueHead;
    
    while (item != NULL)
    {
        if (item == task)
        {
            removeTaskInQueue(item);
            return true;
        }
        
        item = item->nextTask;
    }
    
    return false;
}

void AppRunLoop::removeTaskInQueue(IRunLoopTask *item)
{
    if (item->previousTask != NULL)
        item->previousTask->nextTask = item->nextTask;
    
    if (item->nextTask != NULL)
        item->nextTask->previousTask = item->previousTask;
}

void AppRunLoop::setResetOnUserButton(bool roub)
{
    if (roub)
    {
        //TODO: remove cypress reference here!
        CyPins_SetPinDriveMode(SW_USER, CY_PINS_DM_RES_UP);
        CyPins_SetPin(SW_USER);
        wait_us(10); // wait for pull up resistor
        resetOnUserButton = true;
    }
    else
        resetOnUserButton = false;
}