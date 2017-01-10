// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __mono__application_context__
#define __mono__application_context__

#include "application_context_interface.h"

#include "display/ili9225g/ili9225g.h"
#include "at30ts74_temperature.h"
#include "mono_accelerometer.h"
#include "mono_buzzer.h"

#include "mono_touch_system.h"
//#include "mono_settings.h"
#include "application_run_loop.h"
#include "mono_power_management.h"
#include "mono_rtc.h"


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
        sensor::AT30TS74Temperature at30ts64Sensor;
        sensor::MMAAccelerometer mmaAccelerometer;
        sensor::MonoBuzzer mnBuzzer;
        MonoRTC rtc;
        
        QueueInterrupt UserButton;

        //io::MonoSettings settings;

        ApplicationContext();
        
        void enterSleepMode();
        
        void buttonWakeup();
        
        void sleepForMs(uint32_t ms);
        
        void resetOnUserButton();
        
        void _softwareReset() __attribute((noreturn));
        void _softwareResetToBootloader() __attribute((noreturn));
        void _softwareResetToApplication() __attribute((noreturn));
        
    public:
        
        int exec();
        
        void setMonoApplication(IApplication *monoApp);
    };
    
}


#endif /* defined(__mono__application_context__) */
