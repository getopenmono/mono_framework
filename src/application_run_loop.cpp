// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "application_run_loop.h"
#include "application_context_interface.h"
#include <mbed_debug.h>
#include <wait_api.h>
#include <us_ticker_api.h>
#include "rtc_interface.h"
#include "scheduled_task.h"
#include <consoles.h>

#ifdef DEVICE_SERIAL
extern "C" {
#include <serial_usb_api.h>
extern char serial_usbuart_is_powered;
}
#endif

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
    //debug("mono enter run loop!\r\n");

    checkUsbUartState();

    while (runLoopActive) {

        process();
    }

    //debug("run loop terminated!");
}

void AppRunLoop::process()
{
    checkUsbUartState();

    if (resetOnUserButton)
    {
        if (userBtn == 0)
        {
            debug("Will reset on user button!\r\n");
            wait_ms(300);
            IApplicationContext::SoftwareReset();
        }
    }

    uint32_t start = us_ticker_read();

    //handle touch inputs
    if (IApplicationContext::Instance->TouchSystem != NULL)
        IApplicationContext::Instance->TouchSystem->processTouchInput();

    uint32_t tEnd = us_ticker_read();

    //run dynamic tasks
    processDynamicTaskQueue();

    // run scheduled tasks
    ScheduledTask::processScheduledTasks();
    
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
            //debug("Removing task from dynamic queue!\r\n");
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
        task->nextTask = 0;
    }

    return true;
}

bool AppRunLoop::removeDynamicTask(mono::IRunLoopTask *task)
{
    if (taskQueueHead == 0)
        return false;

    IRunLoopTask *item = taskQueueHead;

    while (item != 0)
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
    if (item->previousTask != 0)
        item->previousTask->nextTask = item->nextTask;

    if (item->nextTask != 0)
        item->nextTask->previousTask = item->previousTask;

    if (taskQueueHead == item)
        taskQueueHead = item->nextTask;
}

void AppRunLoop::checkUsbUartState()
{
    power::IPowerSubSystem *power = IApplicationContext::Instance->PowerManager->PowerSystem;
    bool usbCharge = power->IsUSBCharging();
#ifdef DEVICE_SERIAL
    serial_usbuart_is_powered = usbCharge;
#endif

    if (!usbCharge)
    {
        firstDtrRun = lastDtrValue = true;
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
            debug("mono DTR reboot!\r\n");
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
