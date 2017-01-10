
#ifndef scheduled_task_h
#define scheduled_task_h

#include <queue.h>
#include <date_time.h>
#include <mbed.h>

namespace mono {
    
    class ScheduledTask : IQueueItem
    {
    protected:
        
        static GenericQueue<ScheduledTask> queue;
        
        bool runInSleep;
        DateTime time;
        mbed::FunctionPointer handler;
        
        void runTask(bool inSleep = false);
        bool isDue() const;
        
    public:
        
        ScheduledTask();
        ScheduledTask(const DateTime &scheduledTime);
        
        ScheduledTask(const ScheduledTask &other);
        ScheduledTask &operator=(const ScheduledTask &other);
        
        void reschedule(const DateTime &newTime);
        
        bool willRunInSleep() const;
        void setRunInSleep(bool run);
        
        template <typename Class>
        void setTask(Class *context, void(Class::*memptr)(void))
        {
            handler.attach<Class>(context, memptr);
            if (time.isValid() && time > DateTime::now())
                queue.Enqueue(this);
        }
        
        ~ScheduledTask();
        
        static void processScheduledTasks(bool isSleeping = false);
    };
}

#endif /* scheduled_task_h */
