// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef pct2075_temperature_h
#define pct2075_temperature_h

#include <mbed.h>
#include "temperature_interface.h"
#include "power_aware_interface.h"

namespace mono { namespace sensor {
    
    class PCT2075Temperature : public ITemperature, power::IPowerAware
    {
    private:
        static const uint8_t deviceAddress = 0x76;
        
    protected:
        
        enum Registers
        {
            TEMP_REG = 0,
            CONF_REG = 1,
            THYST_REG = 2,
            TOS_REG = 3,
            TIDLE_REG = 4
        };
        
        uint8_t currentPointerRegister;
        
        bool writeRegister(uint8_t regAddress, uint8_t data);
        
        bool readRegister(uint8_t regAddress, uint16_t *data);
        
        
    public:
        
        PCT2075Temperature();
        
        int Read();
        
    protected:

        void EnablePower();

        // MARK: - Power Aware Interface
        
        virtual void onSystemPowerOnReset();
        
        virtual void onSystemEnterSleep();
        
        virtual void onSystemWakeFromSleep();
        
        virtual void onSystemBatteryLow();
        
    };
    
} }

#endif /* pct2075_temperature_h */
