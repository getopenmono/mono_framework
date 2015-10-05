//
//  ili9225g.hpp
//  mono
//
//  Created by Kristoffer Andersen on 21/09/15.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef ili9225g_h
#define ili9225g_h

#include "../display_controller_interface.h"
#include <rect.h>
#include <power_aware_interface.h>
#include <mbed.h>


namespace mono { namespace display {
    
    class ILI9225G : public IDisplayController, mono::power::IPowerAware
    {
    protected:
        
        mbed::SPI spi;
        mbed::DigitalOut Reset;
        mbed::DigitalOut RegisterSelect;
        mbed::DigitalOut IM0;
        //mbed::InterruptIn TearingOutput;
        
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
    };
    
    
} }

#endif /* ili9225g_h */
