//
//  mn_timer.h
//  mono framework
//
//  Created by Kristoffer Andersen on 16/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef mn_timer_h
#define mn_timer_h

#include <mbed.h>
#include <FunctionPointer.h>
#include "application_run_loop_task_interface.h"


namespace mono {
    
    /**
     * @brief A queued Timer class, recurring or single shot.
     *
     * A timer can call a function at regular intervals or after a defined delay.
     * You can use the timer to do periodic tasks, like house-keeping functions
     * or display updates.
     *
     * ### Queued callback
     *
     * The timer uses the Application Run Loop to schedule the callback handler 
     * function. This means your callback are not executed inside a hardware
     * interrupt context. This is very convenient since your can do any kind of
     * heavy lifting in your callback handler, and your code is not pre-empted.
     *
     * ### Presicion
     *
     * You should note that the timer are not guaranteed to be precisely accurate,
     * it might fire later than your defined interval (or delay). The timer will
     * not fire before your defined interval though.
     * If you use any blocking `wait` statements in your code, you might contribute
     * to loss in precision for timers.
     *
     * If you want precise hardware timer interrupts consider the mbed Ticker 
     * class, but you should be aware of the hazards when using hardware interrupts.
     *
     * ### Example
     *
     * Create a reocurring timer that fires each second:
     * @code
     * Timer timr(1000);
     * timr.attach<MyClass>(this, &MyClass::callback);
     * timr.Start();
     * @endcode
     *
     * The member function `callback` will now be called every second.
     * If you want to use a single shot callback with a delay, Timer has a
     * convenience static function:
     * @code
     * Timer delay = Timer::callOnce<MyClass>(100, this, &MyClass::callback);
     * @endcode
     *
     * Now `delay` is a running timer that calls `callback` only one time. *Note*
     * that the timer object (`delay`) should not be deallocated. Deallocating 
     * the object will cause the timer to shut down.
     *
     * ### Time slices
     *
     * Say you set an interval of 1000 ms, and your callback takes 300 ms to 
     * execute. Then timer will delay for 700 ms and not 1000 ms. It is up to you
     * to ensure your callback do not take longer to execute, than the timer
     * interval.
     */
    class Timer : protected IRunLoopTask
    {
    private:
        
        Timer(const Timer &timer);
        
        Timer& operator=(const Timer &timer);
        
    protected:
        
        mbed::Ticker ticker;
        mbed::FunctionPointer handler;
        
        uint32_t interval;
        
        bool interruptDidFire;
        
        bool running, timerSingleShot;
        
        bool autoRelease;
        
        void taskHandler();
        
        void hwTimerInterrupt();
        
    public:
        
        /**
         * @brief Create a new timer with with an interval or timeout time.
         *
         * All newly created timers are stopped as default. You must also attach
         * callback handler to the timer, before it can start.
         *
         * @param intervalOrTimeoutMs The timers time interval before it fires, in milliseconds
         * @param snglShot Set this to `true` if the timer should only fire once. Default `false`
         */
        Timer(uint32_t intervalOrTimeoutMs, bool snglShot = false);
        
        virtual ~Timer();
        
        /**
         * @brief Start the timer and put into *running* state.
         *
         * **Note**: You must set a callback handler, before starting the timer.
         */
        void Start();
        
        /**
         * @brief Stop the timer, any pending callback will not be executed
         */
        void Stop();
        
        /**
         * See if the timer is single shot.
         */
        bool SingleShot();
        
        /**
         * See if the timer is currently running
         */
        bool Running();
        
        /**
         * @brief Set a new timer interval
         * @param newIntervalMs The timer interval in milliseconds
         */
        void setInterval(uint32_t newIntervalMs);
        
        /**
         * @brief Sets a callback handler function to the timer
         * 
         * @param obj A pointer to the callback member function context (the `this` pointer)
         * @param memPtr A pointer to the member function, that is the callback
         */
        template <typename Owner>
        void setCallback(Owner *obj, void(Owner::*memPtr)(void))
        {
            handler.attach<Owner>(obj, memPtr);
        }
        
        
        void setCallback(void(*cFunction)(void))
        {
            handler.attach(cFunction);
        }
        
        /**
         * @brief Create a single shot timer with a delay and callback function
         * 
         * The timer object is created on the HEAP, which allows it to exists 
         * across stack frame contexts. You can safely create a `callOnce(...)`
         * timer, and return from a function. Even if you do not have a reference
         * to the timer object, it will still run and fire. The timer deallocates
         * itself after it has fired. It cannot be reused.
         *
         * @param delayMs Delay time before the timer fires, in milliseconds.
         * @param obj A pointer to the callbacks function member context (the `this` pointer)
         * @param memPtr A pointer to the callback member function
         * @return A pointer to the single shot timer
         */
        template <typename Owner>
        static Timer* callOnce(uint32_t delayMs, Owner *obj, void (Owner::*memPtr)(void))
        {
            Timer *timer = new Timer(delayMs, true);
            timer->setCallback<Owner>(obj, memPtr);
            timer->autoRelease = true;
            timer->Start();
            
            return timer;
        }
        
    };
    
}

#endif /* mn_timer_h */
