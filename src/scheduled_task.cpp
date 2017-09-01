
#include "scheduled_task.h"
#include "rtc_interface.h"

using namespace mono;

GenericQueue<ScheduledTask> ScheduledTask::queue;

ScheduledTask::ScheduledTask()
{
    runInSleep = false;
}

ScheduledTask::ScheduledTask(const DateTime &scheduledTime)
{
    runInSleep = false;
    time = scheduledTime;
}

ScheduledTask::ScheduledTask(const ScheduledTask &other)
{
    runInSleep = other.runInSleep;
    time = other.time;
    handler = other.handler;
    
    if (time.isValid() && time > DateTime::now() && handler)
        queue.enqueue(this);
}

ScheduledTask::~ScheduledTask()
{
    queue.remove(this);
}

ScheduledTask& ScheduledTask::operator=(const ScheduledTask &other)
{
    runInSleep = other.runInSleep;
    time = other.time;
    handler = other.handler;
    
    if (time.isValid() && time > DateTime::now() && handler)
        queue.enqueue(this);
    
    return *this;
}

void ScheduledTask::reschedule(const mono::DateTime &newTime)
{
    queue.remove(this);
    time = newTime;
    
    if (time.isValid() && time > DateTime::now())
        queue.enqueue(this);
}

bool ScheduledTask::willRunInSleep() const
{
    return runInSleep;
}

void ScheduledTask::setRunInSleep(bool run)
{
    runInSleep = run;
}

void ScheduledTask::runTask(bool inSleep)
{
    queue.remove(this);
    
    if ((inSleep && runInSleep) || !inSleep)
        handler.call();
}

bool ScheduledTask::isDue() const
{
    return time.isValid() && time <= DateTime::now();
}


void ScheduledTask::processScheduledTasks(bool inSleep)
{
    if (!IRTCSystem::__shouldProcessScheduledTasks)
        return;
    
    IRTCSystem::__shouldProcessScheduledTasks = false;
    ScheduledTask *task = ScheduledTask::queue.peek();
    
    while (task != 0)
    {
        // get next before task might be dequeued and re-qeueued
        ScheduledTask *next = queue.next(task);
        
        if (task->isDue())
        {
            task->runTask(inSleep);
        }
        
        task = next;
    }
}

bool ScheduledTask::pendingScheduledTasks(bool inSleep)
{
    ScheduledTask *task = ScheduledTask::queue.peek();

    if (task == 0)
        return false;

    while (task != 0)
    {
        if (task->isDue() && task->handler && (inSleep == false || task->willRunInSleep() == true))
            return true;

        task = queue.next(task);
    }

    return false;
}

