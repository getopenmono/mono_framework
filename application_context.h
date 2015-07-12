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
        
        
        /** Sets a pointer to the mono application object */
        virtual void setMonoApplication(mono::IApplication *app) = 0;
        
        /** Get a pointer to the global application context */
        static ApplicationContext* Instance;
        
    };
}


#endif
