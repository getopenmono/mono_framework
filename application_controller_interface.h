//
//  application_interface.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 27/06/15.
//
//

#ifndef mono_application_interface_h
#define mono_application_interface_h

namespace mono {
    
    /**
     * Every mono application must implement this interface.
     * This is the starting point of the your application code, and is called
     * after the runtime initalization.
     *
     * @brief Abstract interface for a mono application
     * @author Kristoffer Andersen
     */
    class IApplication
    {
    public:
        
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
    };
    
}


#endif
