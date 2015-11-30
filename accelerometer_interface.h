//
//  accelerometer_interface.h
//  accelTest
//
//  Created by Kristoffer Andersen on 27/11/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#ifndef accelerometer_interface_h
#define accelerometer_interface_h

#include <stdint.h>

namespace mono { namespace sensor {
    
    class IAccelerometer
    {
    public:
        
        virtual void Start() = 0;
        virtual void Stop() = 0;
        
        virtual bool IsActive() = 0;
        
        virtual int16_t rawXAxis() = 0;
        
    protected:
        
        
        virtual bool readRegister(uint8_t regAddr, uint8_t *data) = 0;
        virtual bool readRegister(uint8_t regAddr, uint16_t *data) = 0;
        
        virtual bool writeRegister(uint8_t regAddr, uint8_t data) = 0;
    };
    
} }

#endif /* accelerometer_interface_h */
