//
//  temperature_interface.h
//  tempTest
//
//  Created by Kristoffer Andersen on 30/11/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#ifndef temperature_interface_h
#define temperature_interface_h

namespace mono { namespace sensor {
    
    class ITemperature
    {
    public:
        
        /**
         * Reads the current temperature from the temperature sensor
         * @return the temperature in celcius (integers)
         */
        virtual int Read() = 0;
        
    };
    
} }

#endif /* temperature_interface_h */
