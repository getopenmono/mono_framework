//
//  cy_gpio.c
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "cy_gpio.h"

void CyDelayUs(int us)
{
    
}

void CyDelay(int ms)
{
    
}

struct cy_pin pinList[100];

void CyPins_SetPin(int pin)
{
    pinList[pin].val = 1;
}

void CyPins_ClearPin(int pin)
{
    pinList[pin].val = 0;
}

int  CyPins_ReadPin(int pin)
{
    return pinList[pin].val;
}

void CyPins_SetPinDriveMode(int pin, int mode)
{
    pinList[pin].mode = mode;
}

int CyPins_GetPinDriveMode(int pin)
{
    return pinList[pin].mode;
}