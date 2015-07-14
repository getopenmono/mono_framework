//
//  application_interface.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 27/06/15.
//
//

#ifndef mono_application_interface_h
#define mono_application_interface_h

#include <application_context_interface.h>

namespace mono {
    
    /**
     * Every mono application must implement this interface.
     * This is the starting point of the your application code, you must call it
     * after the runtime initalization.
     *
     * You do this from inside the `main()` function. Your main function should
     * look like this:
     *
     * ``
     * int main()
     * {
     *     MyIApplicationSubclass appCtrl;
     *
     *     return appCtrl.enterRunLoop()
     * }
     * ``
     * 
     * Your mono applications entry point must be your own subclass of 
     * IApplication. And you _must_ initalize it inside (not outside)
     * the `main()` function. This is strictly nessesary, because the
     * @ref IApplicationContext must be ready when the IApplication is executed.
     *
     * Also you must call the @ref enterRunLoop method from main, to enter the
     * event loop and prevent `main()` from returning.
     *
     * @brief Abstract interface for a mono application
     * @author Kristoffer Andersen
     */
    class IApplication
    {
    public:
        
        /**
         * Constructor for the global Application Controller.
         * See @ref IApplication for a describtion on when to call this 
         * constructor.
         *
         * @brief Construct the global Application class
         */
        IApplication()
        {
            IApplicationContext::Instance->setMonoApplication(this);
        }
        
        /**
         * Called when mono boot after having been power off or after a reset
         * This method is only called once, you should use it to do inital
         * data and object setup.
         *
         * When this method returns mono will enter in an event loop, so use this
         * method to setup event listeners for your code.
         *
         * Do not call this method yourself, it is intended only to be called by
         * the mono framework runtime.
         */
        virtual void monoWakeFromReset() = 0;
        
        
        /**
         * The runtime library calls this function when the MCU will go into
         * standby or sleep mode.
         * Use this method to disconnect from networks or last-minute clean ups.
         * 
         * When you return from this method the system will goto sleep, and at
         * wakeup the @ref monoWakeFromSleep() method will be called automatically.
         *
         * Do not call this method yourself, it is ontended only to be called by
         * the mono framework runtime.
         */
        virtual void monoWillGotoSleep() = 0;
        
        /**
         * Called when mono comes out of a standby or sleep state, where 
         * the MCU instruction execution has been paused.
         *
         * Use this method to reestablish I/O connections and refresh data 
         * objects.
         *
         * You should not call this method your self, it is intended only to be
         * called by the mono framework runtime.
         */
        virtual void monoWakeFromSleep() = 0;
        
        /**
         * Start the main run loop for your mono application. This method calls 
         * the global @ref IApplicationContext run loop.
         *
         * The last line in the main.cpp file must be a call to this function:
         *
         * ``
         * int main()
         * {
         *     MyIApplicationSubclass appCtrl;
         *
         *     // ** Some app ctrl setup code here perhaps?
         *     
         *     return appCtrl.enterRunLoop();
         * }
         * ``
         *
         * @brief Start the mono application run loop
         * @returns The run loop never returns, the return type is only for comformaty.
         */
        int enterRunLoop()
        {
            return IApplicationContext::Instance->exec();
        }
    };
    
}


#endif
