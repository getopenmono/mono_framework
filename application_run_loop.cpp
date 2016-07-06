// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "application_run_loop.h"
#include "application_context_interface.h"
#include <mbed.h>
#include <consoles.h>

#include "display/ui/console_view.h"
extern mono::ui::ConsoleView<176,110> *ui_console_stdout;

using namespace mono;

AppRunLoop::AppRunLoop() : userBtn(SW_USER, 1, PullUp)
{
    runLoopActive = true;
    lastDtrValue = true;
    resetOnDTR = true;
    resetOnUserButton = false;
    taskQueueHead = NULL;
    firstDtrRun = true;
}

void AppRunLoop::exec()
{
    debug("mono enter run loop!\n\r");

    checkUsbUartState();
    
    while (runLoopActive) {
        
        process();
    }
    
    debug("run loop terminated!");
}

void AppRunLoop::process()
{
    checkUsbUartState();
    
    if (resetOnUserButton)
    {
        if (userBtn == 0)
        {
            debug("Will reset on user button!\n\r");
            wait_ms(300);
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

void AppRunLoop::CheckUsbDtr()
{
    checkUsbUartState();
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

void AppRunLoop::checkUsbUartState()
{
    power::IPowerSubSystem *power = IApplicationContext::Instance->PowerManager->PowerSystem;
    bool usbCharge = power->IsUSBCharging();

    if (!usbCharge)
    {
        return;
    }

    if (firstDtrRun)
    {
        firstDtrRun = false;
        if (mono::defaultSerial.writeable())
        {
            lastDtrValue = mono::defaultSerial.DTR();
        }
        else
        {
            lastDtrValue = false;
        }

        return;
    }

    if (mono::defaultSerial.writeable())
    {
        
        bool dtr = mono::defaultSerial.DTR();
        if (resetOnDTR && (!dtr) && lastDtrValue)
        {
            debug("mono DTR reboot!\n\r");
            wait_ms(20);
            IApplicationContext::SoftwareReset();
        }
        
        lastDtrValue = dtr;
    }
}

void AppRunLoop::setResetOnUserButton(bool roub)
{
    if (roub)
    {
        //TODO: remove cypress reference here!
        userBtn.setMode(PullUp);
        userBtn = 1;
        wait_us(10); // wait for pull up resistor
        resetOnUserButton = true;
    }
    else
        resetOnUserButton = false;
}

void AppRunLoop::quit()
{
    runLoopActive = false;
}