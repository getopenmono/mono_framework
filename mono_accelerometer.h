//
//  mono_accellerometer.h
//
//
//  Created by Kristoffer Andersen on 27/11/2015.
//  Copyright © 2015 your name. All rights reserved.
//


#ifndef mono_accellerometer_h
#define mono_accellerometer_h

#include "accelerometer_interface.h"

namespace mono { namespace sensor {
    
    class MMAAccelerometer : public IAccelerometer
    {
    private:
        static const uint8_t accelAddress = 0x1D;
        
    public:
        
        MMAAccelerometer();
        
        void Start();
        void Stop();
        
        bool IsActive();
        
        virtual int16_t rawXAxis();
        
        enum Registers
        {
            STATUS      = 0x00,
            OUT_X_MSB   = 0x01,
            OUT_X_LSB   = 0x02,
            OUT_Y_MSB   = 0x03,
            OUT_Y_LSB   = 0x04,
            OUT_Z_MSB   = 0x05,
            OUT_Z_LSB   = 0x06,
            WHO_AM_I    = 0x0D,
            XYZ_DATA_CFG= 0x0E,
            CTRL_REG1   = 0x2A
        };
        
    protected:
        
        
        
        virtual bool readRegister(uint8_t regAddr, uint8_t *data);
        virtual bool readRegister(uint8_t regAddr, uint16_t *data);
        
        bool writeRegister(uint8_t regAddr, uint8_t data);
    };
    
} }

#endif /* mono_accellerometer_h */
