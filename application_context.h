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
#include <application_interface.h>

namespace mono {
    
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
    class ApplicationContext
    {
    public:
        /**
         * Pointer to the displat interface controller object. The object itself
         * should be initialized differntly depending on the ApplicationContext
         *
         */
        display::IDisplayController *displayController;
        
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
        virtual void exec() = 0;
        
        
        /** Sets a pointer to the mono application object */
        virtual void setMonoApplication(mono::IApplication *app) = 0;
        
        /** Get a pointer to the global application context */
        static ApplicationContext* Instance;
        
    };
}


#endif
