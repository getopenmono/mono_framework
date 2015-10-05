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
//#include <hx8340.h>
#include "display/ili9225g/ili9225g.h"
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
        //mono::display::HX8340 dispController;
		
        ApplicationContext();
        
        void enterSleepMode();
        void sleepForMs(uint32_t ms);
        
    public:
        
        
        
        int exec();
        
        void setMonoApplication(IApplication *monoApp);
        
    };
    
}


#endif /* defined(__usbtest__application_context__) */
