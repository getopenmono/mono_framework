//
//  application_run_loop.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//
//

#ifndef __displaySimTest__application_run_loop__
#define __displaySimTest__application_run_loop__

#include <mono.h>


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
        bool lastDtrValue;
        
        
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
        
        AppRunLoop();
        
        void exec();
    };
    
}


#endif /* defined(__displaySimTest__application_run_loop__) */
