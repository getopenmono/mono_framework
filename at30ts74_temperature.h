//
//  at30ts74_temperature.hpp
//  tempTest
//
//  Created by Kristoffer Andersen on 26/01/2016.
//  Copyright © 2016 your name. All rights reserved.
//

#ifndef at30ts74_temperature_h
#define at30ts74_temperature_h

#include <stdint.h>
#include "temperature_interface.h"

namespace mono { namespace sensor {
    
    class AT30TS74Temperature : public ITemperature
    {
    private:
        static const uint8_t deviceAddress = 0x48;
        
    protected:
        
        enum Registers
        {
            TEMPERATURE_REG = 0,
            CONFIGURATION_REG = 1,
            T_LOW_LIMIT_REG = 2,
            T_HIGH_LIMIT_REG = 3
        };
        
        uint8_t currentPointerRegister;
        
        bool writeRegister(uint8_t regAddress, uint8_t data);
        
        bool readRegister(uint8_t regAddress, uint16_t *data);
        
    public:
        
        AT30TS74Temperature();
        
        int Read();
        int ReadMilliCelcius();
    };
    
} }

#endif /* at30ts74_temperature_h */
