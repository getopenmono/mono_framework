//
//  application_run_loop_task_interface.h
//  mono
//
//  Created by Kristoffer Andersen on 07/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef mono_application_run_loop_task_interface_h
#define mono_application_run_loop_task_interface_h


namespace mono {
    class AppRunLoop;
    /** 
     * THis interface defines tasks or functions that can be inserted into the 
     * @ref ApplicationRunLoop.
     *
     * The interface defines a @taskHandler method that implements the actual
     * logic. Also, the interface defines the pointers `previousTask` and
     * `nextTask`. These define the previous and next task to be run, in the run
     * loops task queue.
     *
     * To avoid dynamic memory allocation of linked lists and queues in the run
     * loop, the run loop handler functions, are themselves items in a linked
     * list.
     *
     * All classes that that want to use the run loop, must inherit this 
     * interface.
     *
     * *NOTE* that tasks in the run loop do not have any contraints on how often
     * or how rare they are executed. If you need a function called at fixes
     * intervals, use a Ticker or timer.
     */
    class IRunLoopTask
    {
        friend class AppRunLoop;
    protected:
        /**
         * A pointer to the previous task in the run loop
         * The the task is the first in queue, this is NULL
         */
        IRunLoopTask *previousTask;
        
        /**
         * A pointer to the next task to be run, after this one.
         * If this task is the last in queue, this is NULL
         */
        IRunLoopTask *nextTask;
        
        /**
         * Tasks are expected to be repetative. They are scheduled over and over
         * again. Set this property to `true` and the task will not scheduled 
         * again, when handled.
         */
        bool singleShot;
        
        /**
         * This is the method that gets called by the run loop.
         * 
         * *NOTE* that this is not an interrupt function, you can do stuff that
         * take some time.
         */
        virtual void taskHandler() = 0;
        
    };
}


#endif
