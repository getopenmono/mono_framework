//
//  application_context.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/07/15.
//
//

#ifndef displaySimTest_application_context_h
#define displaySimTest_application_context_h

#include <display_controller_interface.h>
#include <application_run_loop.h>
#include "power_system_interface.h"

namespace mono {
    
    class IApplication; // forward declaretion of the IApplication interface
    
    /**
     * The Application context class is a singleton class that is automatically 
     * instanciated by the framework. You should not need to interact with it
     * directly. It is allocated on the stack, with its member objects.
     *
     * The application context controls the application event loop at hardware
     * event inputs. It is essential for communicating with Serial-USB and the
     * display.
     *
     * Depending on the execution context (hardware mono device or simulator),
     * different subclasses of this interface it used. This interface is provided
     * to give your application code a pointer to the concrete implementation
     * of the application context. Regardless of running on a simulator or the 
     * actual device.
     *
     */
    class IApplicationContext
    {
    protected:
        
        /**
         * Subclasses should overirde this method to make the sysetm goto sleep
         */
        virtual void enterSleepMode() = 0;
        
        /**
         * Subclasses should override this to enable sleep mode for a specefic
         * interval only.
         */
        virtual void sleepForMs(uint32_t ms) = 0;
        
    public:
        /**
         * Pointer to the displat interface controller object. The object itself
         * should be initialized differntly depending on the ApplicationContext
         *
         */
        display::IDisplayController *displayController;
        
        /**
         * A pointer to the initialized power sub-system. This is initialize
         * automatically and depends on compiled environment.
         * The power system to used to control power supply to periphirals and
         * to give interrupt on power related events.
         *
         * **WARNING**: Use this class with extreme caution! Wrong power
         * settings can fry the MCU and other peripherals!
         */
        power::IPowerSystem *PowerSystem;
        
        /**
         * A reference to the main run loop of the application.
         * This pointer must be instanciated be subclasses
         */
        AppRunLoop *RunLoop;
        
        /**
         * This method starts the global run/event loop for the mono application.
         * The method never returns, so a call to this function should be the
         * last line in your `main()` function.
         *
         * The event loop automatically schedules the sub system, such as the
         * network, inputs and the display.
         *
         * @brief Start the application run loop
         */
        virtual int exec() = 0;
        
        /** Sets a pointer to the mono application object */
        virtual void setMonoApplication(mono::IApplication *app) = 0;
        
        /**
         * The mono application controller should call this to give the
         * Application Context a reference to itself.
         *
         * This will ensure the Application Controllers methods gets called.
         */
//        static void SetMonoApplication(mono::IApplication &app)
//        {
//            IApplicationContext::Instance->setMonoApplication(&app);
//        }
        
        /**
         * Call this method to make mono goto sleep.
         * 
         * In sleep mode the CPU does not excute instruction and powers down into
         * a low power state.
         * The power system will turn off dynamically powered peripherals.
         *
         * *NOTE*: Before you call this method make sure that you configured a way
         * to go out of sleep.
         */
        static void EnterSleepMode()
        {
            IApplicationContext::Instance->enterSleepMode();
        }
        
        /**
         * Enter MCU sleep mode for a short time only. Sets a wake-up timer us the
         * preferred interval, and calls the @ref EnterSleepMode method.
         * @param ms The number of milli-second to sleep
         */
        static void SleepForMs(uint32_t ms)
        {
            IApplicationContext::Instance->sleepForMs(ms);
        }
        
        /** Get a pointer to the global application context */
        static IApplicationContext* Instance;
        
    };
}


#endif
