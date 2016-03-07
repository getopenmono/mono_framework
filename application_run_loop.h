//
//  application_run_loop.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//
//

#ifndef __displaySimTest__application_run_loop__
#define __displaySimTest__application_run_loop__
#include <stdint.h>

#include <application_run_loop_task_interface.h>
#include "mn_digital_out.h"

namespace mono {
    
    /**
     * This is the event run-loop for all mono applications. This class is 
     * instantiated and used inside the @ref IApplicationContext interface. You
     * should not interact with this class directly.
     *
     * The run loop handles non-critical periodicly tasks. Classes can install
     * tasks in the run-loop. Such classes are usually repetitive timers or lazy
     * interrupt handlers.
     *
     * Some standard system tasks are handled staticly inside the loop, like the
     * USB serial reads.
     */
    class AppRunLoop
    {
    protected:
        
        /**
         * As long as this i `true` the stadard run loop will run
         * 
         * If set to `false`, the run loop will exit, and mono might will enter
         * a low power state.
         * TODO: power safe modes and run loops?
         */
        bool runLoopActive;
        
        /**
         * The last seen serial DTR value. Reset can only happen in transitions.
         */
        bool lastDtrValue, firstDtrRun;
        
        /**
         * Set to `true` if you want the run loop to call software reset when 
         * pressing the user button.
         * Initial value is `false`
         */
        bool resetOnUserButton;
        
        /**
         * A pointer to the head task of the dynamic task queue.
         * If no task are in the queue, this is NULL
         */
        IRunLoopTask *taskQueueHead;
        
        
        /**
         * Execute all tasks in the dynamic task queue
         * 
         */
        void processDynamicTaskQueue();
        
        /** Internal method to sow together neightbourghs in the linked list */
        void removeTaskInQueue(IRunLoopTask *task);
        
        io::DigitalOut userBtn;
        
        /** Process a single iteratio of the run loop */
        void process();

        /** read the UART DTR state if possible */
        void checkUsbUartState();
        
    public:
        
        /**
         * As default behaviour the run loop will force a reset on high-to-low
         * transition on the serial ports DTR (Data Terminal Ready) line.
         *
         * This property controls this feature, setting it to `true` will enable
         * software reset via the serial connection. This means the *monoprog*
         * programmer can reset the device and connect to the bootloader.
         *
         * Setting this to `false` means *monoprog* cannot automatically reset
         * into the bootloader, you must press the reset button yourself.
         *
         */
        bool resetOnDTR;
        
        
        /**
         * @brief The CPU time used on proccessing touch input.
         * This includes:
         * 
         * * ADC sampling (approx 16 samples)
         * * Touch value evaluation, and possible convertion into events
         * * Traversing the responder chain
         * * Handling TouchBegin, TouchEnd & TouchMove, and any function they call
         *
         * This time includes the execution of your code if you have any button
         * handlers or touch based event callbacks.
         */
        uint32_t TouchSystemTime;
        
        /**
         * @brief The CPU time used on processing the dynamic task queue
         * The time spend here include all queued tasks and callbacks.
         * these could be:
         *
         * * Timer callback
         * * Any @ref QueueInterrupt you might have in use
         * * All display painting routines (repainting of views subclasses)
         * * Any custom active @ref IRunLoopTask you might use
         *
         * Nearly all callbacks are executed with origin inside the dynamic task
         * queue. Expect that the majority of your code are executed here.
         */
        uint32_t DynamicTaskQueueTime;
        
        
        AppRunLoop();
        
        
        /**
         * Start executing the run loop.
         * 
         */
        void exec();
        
        /**
         * Add a task to the dynamic task queue. This task is repeated over and
         * over, until it reports that its should not be scheduled.
         * 
         * The task is added to a linked list, runtime is *n*.
         * @return Always true at this point
         */
        bool addDynamicTask(IRunLoopTask *task);
        
        /**
         * Remove a task from the dynamic task queue. This will search the queue
         * for he pointer provided, and remove it.
         * @param task A pointer to the object, that should be removed
         * @return `true` if the object was found and removed, `false` otherwise. 
         */
        bool removeDynamicTask(IRunLoopTask *task);
        
        /**
         * @brief Sets the *Reset on User Button* mode
         * 
         * If `true` the run loop will check the user button, and if pressed
         * it will trigger a software reset.
         * @param roub `true` will reset on user button, `false` is normal functionality.
         */
        void setResetOnUserButton(bool roub);
        
        /**
         * Terminate the run loop. 
         * Application events and more will stop working
         * 
         * You should use this, if you use your own embedded run loops.
         */
        void quit();
    };
    
}


#endif /* defined(__displaySimTest__application_run_loop__) */
