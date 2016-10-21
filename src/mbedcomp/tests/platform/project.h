//
//  project.h
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef project_h
#define project_h

#define CyGlobalIntEnable
#define Bootloadable_SET_RUN_TYPE(type)

void CyDelayUs(int us);
void CyDelay(int ms);
void CySoftwareReset();


#include <pins.h>
#include <cy_gpio.h>
#include <cy_spi.h>
#include <cy_pwm.h>
#include <cy_i2c.h>



#endif /* project_h */
