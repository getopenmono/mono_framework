//
//  application_context.h
//  mono
//
//  Created by Kristoffer Andersen on 13/07/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __mono__application_context__
#define __mono__application_context__

#include <application_context_interface.h>

#include "display/ili9225g/ili9225g.h"
#include "mono_touch_system.h"

#include <application_run_loop.h>
#include "mono_power_management.h"


namespace mono {
    
    class ApplicationContext : public IApplicationContext
    {
    protected:
        
        IApplication *application;
        
        static ApplicationContext singleton;
        
        AppRunLoop runLoop;
        power::MonoPowerManagement pwrMgmt;
        
        //Depends on PowerMgmt
        mono::display::ILI9225G dispController;
        MonoTouchSystem touchSys;
		
        QueueInterrupt UserButton;
        
        ApplicationContext();
        
        void enterSleepMode();
        void sleepForMs(uint32_t ms);
        
        void resetOnUserButton();
        
        void _softwareReset() __attribute((noreturn));
        void _softwareResetToBootloader() __attribute((noreturn));
        
    public:
        
        
        
        int exec();
        
        void setMonoApplication(IApplication *monoApp);
    };
    
}


#endif /* defined(__mono__application_context__) */
