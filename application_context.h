//
//  application_context.h
//  usbtest
//
//  Created by Kristoffer Andersen on 13/07/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __usbtest__application_context__
#define __usbtest__application_context__

#include <application_context_interface.h>

#ifdef MONO_DISP_CTRL_HX8340
#include "display/hx8340/hx8340.h"
#else
#include "display/ili9225g/ili9225g.h"
#include "mono_touch_system.h"
#endif

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
#ifdef MONO_DISP_CTRL_HX8340
        mono::display::HX8340 dispController;
#else
        mono::display::ILI9225G dispController;
        MonoTouchSystem touchSys;
#endif
		
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


#endif /* defined(__usbtest__application_context__) */
