
#ifndef scheduled_task_h
#define scheduled_task_h

#include <queue.h>
#include <date_time.h>
#include <mbed.h>

namespace mono {

    /**
     * @brief Schedule function a call at a point in the future using the RTC
     * 
     * A SchdueldTask is a wrapper around a function call that is scheduled some
     * time in the future. You create a task by defining a function to be called,
     * and a @ref DateTime on when it should be called.
     *
     * A ScheduledTask differs from @ref Timer by not being repetitive by nature.
     * Also, the time resolution on is seconds, not milliseconds as on Timers.
     *
     * An advantage of scheduled tasks is they can execute in sleep mode. They have
     * the ability to wake Mono from sleep, either completely or just to execute
     * simple house keeping code inside the sleep mode state!
     *
     * Scheduled tasks rely on the RTC system to execute. They use the global 
     * system @ref DateTime object to execute when they are due. A ScheduledTask
     * executes once. If you wish to re-scedule the task at a new point in the 
     * future, use the method @ref reschedule.
     *
     * ## Execution in Sleep Mode
     *
     * You can opt-in to allow your task to run in sleep mode. This means Mono's
     * CPU wake up and executes your task's callback function. You enabled sleep
     * mode execution by:
     *
     * @code
     *  ScheduledTask task(DateTime::now().addMinutes(30));
     *  task.setRunInSleep(true);
     *  task.setTask<MyClass>(this, &MyClass::myCallback);
     * @endcode
     *
     * The function is executed without exiting the frameworks sleep state. This
     * means the auxillary power is not enabled. (Display, Wireless chip and 
     * sensors are off.)
     * Inside you callback function you can do small lightweight tasks, and when
     * your function returns Mono re-enter the sleep state.
     *
     * ## Wake-up from a task
     *
     * If you want to wake Mono from sleep, you must indicate to the 
     * @ref IPowerManager that sleep mode should be exited. This is done by 
     * setting the flag:
     *
     * @code
     * IApplicationContext::Instance->PowerManager->__shouldWakeUp = true;
     * @endcode
     *
     * This will trigger a full wake-up, just as toggling the user button does.
     *
     * ## RTC must run
     *
     * Scheduled tasks _will not_ work if the RTC system is not started. This
     * system is automatically started on reset, so the system is enabled by
     * default.
     * 
     * Just like @ref Timer you are not guaranteed to have you function executed
     * at the exact moment defined by the @ref DateTime provided. The guarantee
     * is that your function will not be executed _before_ the provided time
     * stamp.
     */
    class ScheduledTask : public IQueueItem
    {
    protected:

        /** The global singleton queue of pending tasks */
        static GenericQueue<ScheduledTask> queue;
        
        bool runInSleep;
        DateTime time;
        mbed::FunctionPointer handler;

        /** Execute the tasks callback function */
        void runTask(bool inSleep = false);

        /** Return `true` if the tasks time stamp has been reached */
        bool isDue() const;
        
    public:

        /**
         * @brief Consrtuct an empty (non-executable) Task
         * 
         * You must provide a callback function and reschedule the task using
         * the accessors in subsequent calls.
         */
        ScheduledTask();

        /**
         * @brief Construct a task that is due at a provided time stamp
         * 
         * Create a ScheduledTask that is due on the given @ref DateTime, which
         * must be in the future. You must also provide the callback function
         * using the @ref setTask method.
         *
         * @param scheduledTime The point in the future where the task should run
         */
        ScheduledTask(const DateTime &scheduledTime);
        
        ScheduledTask(const ScheduledTask &other);
        ScheduledTask &operator=(const ScheduledTask &other);

        /**
         * @brief Reschedules the task at a new point in the future
         * 
         * Tasks only executes once. To enable re-ocurring tasks, use this method
         * to schedule the task again.
         *
         * @param newTime The new point in the future where the task is run again
         */
        void reschedule(const DateTime &newTime);

        /**
         * @brief Get is this task will execute in sleep mode
         */
        bool willRunInSleep() const;

        /**
         * @brief Allow this task to run in sleep mode
         * 
         * If you set this to `true` the task will execute in sleep mode. Note 
         * that it does not abort sleep (wake up), it execute inside the sleep
         * mode.
         *
         * Task that do not run in sleep are executed ASAP after wake-up.
         *
         * @param run `true` if the task can run in sleep, `false` otherwise
         */
        void setRunInSleep(bool run);

        /**
         * @brief Set the tasks callback function
         * 
         * Here you provide the function that is called when the task is 
         * scheduled
         *
         * @param context The `this` pointer or object instance
         * @param memptr A pointer to a member method on a class
         */
        template <typename Class>
        void setTask(Class *context, void(Class::*memptr)(void))
        {
            handler.attach<Class>(context, memptr);
            if (time.isValid() && time > DateTime::now())
                queue.Enqueue(this);
        }
        
        ~ScheduledTask();

        /**
         * @brief System routine of run any due scheduled task
         * 
         * the RTC system automatically calls this method, you should not call
         * it yourself.
         */
        static void processScheduledTasks(bool isSleeping = false);

        /**
         * @brief Returns `true` if there are scheduling tasks pending for 
         * processing.
         *
         * This means a running task has timed out and are ready to have its
         * handler called. tasks with no callback handler are not regarded as
         * pending.
         *
         * @param inSleep If this static method is called from inside sleep mode, set to `true`
         */
        static bool pendingScheduledTasks(bool inSleep = false);
    };
}

#endif /* scheduled_task_h */
