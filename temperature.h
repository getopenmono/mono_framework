//
//  temperature.h
//
//
//  Created by Kristoffer Andersen on 30/11/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#ifndef temperature_h
#define temperature_h

#include "at30ts74_temperature.h"

namespace mono { namespace sensor {
    static AT30TS74Temperature at30ts74Sensor;
    //ITemperature *Temperature = &at30ts74Sensor;
} }

#endif /* temperature_h */
