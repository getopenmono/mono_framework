// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef ili9225g_h
#define ili9225g_h

#include "../display_controller_interface.h"
#include <rect.h>
#include "queue_interrupt.h"
#include "mn_timer.h"
#include <power_aware_interface.h>
#include "power_fenced_peripheral_interface.h"
#include "mn_digital_out.h"
#include <mbed.h>


namespace mono { namespace display {
    
    class ILI9225G : public IDisplayController, mono::power::IPowerAware, IRunLoopTask
    {
    protected:
        
        mbed::SPI spi;
        mono::io::DigitalOut Reset;
        mbed::DigitalOut RegisterSelect;
        mbed::DigitalOut IM0;
        mbed::InterruptIn tearingEffect;
        mbed::Ticker tearingWatchdog;
        bool tearingInterruptPending;
        bool rebootDisplay;
        uint32_t teWat, watTime;
        
        uint8_t dr, dm0,dm1,dm2;
        
        mono::geo::Rect curWindow;
        
        void writeData(uint16_t data);
        
        void writeRegister(uint16_t regData);
        
        void writeCommand(uint16_t regData, uint16_t data);
        
        /// Power awareness protocol
        
        void onSystemPowerOnReset();

        void onSystemEnterSleep();

        void onSystemWakeFromSleep();

        void OnSystemBatteryLow();
        
        void tearingEffectHandler();
        
        void tearingWatchdogHandler();
        
        void taskHandler();
        
    public:
        
        ILI9225G();
        
        void init();
        
        void setWindow(int x, int y, int width, int height);
        
        uint16_t ScreenWidth() const;
        uint16_t ScreenHeight() const;
        
        void setCursor(int x, int y);
        
        int getCursorX();
        int getCursorY();
        
        void write(Color pixelColor);
        uint16_t read();
        
        void setBrightness(uint8_t value);
        uint8_t Brightness() const;
    };
    
    
} }

#endif /* ili9225g_h */
