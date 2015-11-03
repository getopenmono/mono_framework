//
//  queue_interrupt.h
//  mono
//
//  Created by Kristoffer Andersen on 13/10/15.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef queue_interrupt_h
#define queue_interrupt_h

#include <InterruptIn.h>
#include "application_run_loop_task_interface.h"

namespace mono {
    
    /**
     * @brief An queued input pin interrupt function callback handler
     * This class represents an input pin on mono, and provides up to 3 different
     * callback handler functions. You can installed callback function for rising,
     * falling or both edges.
     *
     * ### Queued interrupts
     * In Mono framework a queued interrupt is handled inside the normal 
     * execution context, and not the hardware interrupt routine. In embedded
     * programming it is good practice not to do any real work, inside the hardware
     * interrupt routine.
     * Instead the best practice method is to set a signal flag, and handled the
     * event in a run loop.
     *
     * QueueInterrupt does this for you. The @ref rise, @ref fall and @ref change
     * callback are all executed by the default mono run loop (@ref AppRunLoop)
     * You can safely do heavy calculations or use slow I/O in the callback
     * routines you assign to QueueInterrupt!
     *
     * ### Latency
     * The run loop might handle the interrupt callback some time after it occur,
     * if it is busy doing other stuff. THerefore you cannot expect to have your
     * callback executed the instant the interrupt fires. (If you need that use
     * @ref DirectInterrupt)
     * `QueueInterrupt` holds the latest interrupt trigger timestamp, to help you
     * determine the latency between the actual interrupt and you callback.
     * Also, many interrupt triggering signal edges might occur, before the run loop
     * executes you handler. The timestamp only shows the latest one.
     */
    class QueueInterrupt : public mbed::InterruptIn, public IRunLoopTask
    {
    protected:
        
        bool addedToRunLoop, fallEvent, riseEvent, snapShot, deactivateUntilHandler, isHandled;
//        bool changeEvent;
        uint32_t riseTimeStamp, fallTimeStamp;
        
//        mbed::FunctionPointer _change, _queue_change;
        mbed::FunctionPointer _queue_rise, _queue_fall;
        
        void taskHandler();
        void activateQueueTaskHandler();
        void deactivateQueueTaskHandler();
        
        void _irq_rise_handler();
        void _irq_fall_handler();
//        void _irq_change_handler();
        
    public:
        
        /**
         * Assign a queued inetrrupt handler to a physical pin
         * @param inputPinName The actual pin to listen on (must be PORT0 - PORT12)
         * @param mode OPTIONAL: The pin mode, default is Hi-Impedance input.
         */
        QueueInterrupt(PinName inputPinName = NC, PinMode mode = PullNone);
        ~QueueInterrupt();
        
        /**
         * Set this property to `true`, to turn off incoming interrupts while
         * waiting for the run loop to finish process a pending interrupt.
         * 
         * If you want to do heavy calculations or loading in your interrupt
         * function, you might want to not queue up new interrupts while you
         * process a previous one.
         * @param OPTIONAL: Set this to false, to *not* disable interrupts while processing. Default is `true`
         */
        void DeactivateUntilHandled(bool deactive = true);
        
        /**
         * Get the state of the @ref DeactivateUntilHandled property. If `true`
         * the hardware interrupt is deactivated until the handler has run.
         * If `false` (the default when constructing the object), all interrupt 
         * are intercepted, and will be handled. This means the handler can be
         * executed two times in row.
         * @returns `true` if incomming interrupt are displaed, until previous is handled.
         */
        bool IsInterruptsWhilePendingActive() const;
        
        /** Attach a function to call when a rising edge occurs on the input
         *
         *  @param fptr A pointer to a void function, or 0 to set as none
         */
        void rise(void (*fptr)(void));
        
        /** Attach a member function to call when a rising edge occurs on the input
         *
         *  @param tptr pointer to the object to call the member function on
         *  @param mptr pointer to the member function to be called
         */
        template<typename T>
        void rise(T* tptr, void (T::*mptr)(void)) {
            _queue_rise.attach(tptr, mptr);
            _rise.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_rise_handler);
            activateQueueTaskHandler();
            gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
        }
        
        /** Attach a function to call when a falling edge occurs on the input
         *
         *  @param fptr A pointer to a void function, or 0 to set as none
         */
        void fall(void (*fptr)(void));
        
        /** Attach a member function to call when a falling edge occurs on the input
         *
         *  @param tptr pointer to the object to call the member function on
         *  @param mptr pointer to the member function to be called
         */
        template<typename T>
        void fall(T* tptr, void (T::*mptr)(void)) {
            _queue_fall.attach(tptr, mptr);
            _fall.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_fall_handler);
            activateQueueTaskHandler();
            gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
        }
        
        /**
         * On fall interrupts, this is the µSec. ticker timestamp for the 
         * falling edge inetrrupt.
         * You can use this to calculate the time passed from the interrupt
         * occured, to the time you process it in the application run loop.
         * @returns The ticker time of the falling edge in micro seconds
         */
        uint32_t FallTimeStamp();
        
        /**
         * On rise interrupts, this is the µSec. ticker timestamp for the
         * rising edge inetrrupt.
         * You can use this to calculate the time passed from the interrupt
         * occured, to the time you process it in the application run loop.
         * @returns The ticker time of the rising edge in micro seconds
         */
        uint32_t RiseTimeStamp();
        
        /**
         * @brief The pin value at the moment the H/W interrupt triggered
         * The callback might be executed some time after the actual inetrrupt
         * occured. THis method return the pin state at the moment of the interrupt.
         * @returns The pin state, at the time of the interrupt
         */
        bool Snapshot();
        
//        /** Attach a function to call when a rising or falling edge occurs on the input
//         *
//         *  @param fptr A pointer to a void function, or 0 to set as none
//         */
//        void change(void (*fptr)(void));
        
        /** Attach a member function to call when a rising or falling edge occurs on the input
         *
         *  @param tptr pointer to the object to call the member function on
         *  @param mptr pointer to the member function to be called
         */
        template<typename T>
        void change(T* tptr, void (T::*mptr)(void)) {
            _queue_fall.attach(tptr, mptr);
            _fall.attach<QueueInterrupt>(this, &QueueInterrupt::_irq_fall_handler);
            activateQueueTaskHandler();
            gpio_irq_set(&gpio_irq, (gpio_irq_event)3, 1);
        }
        
        
    };
}

#endif /* queue_interrupt_h */
