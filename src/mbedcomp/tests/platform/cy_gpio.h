//
//  cy_gpio.h
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef cy_gpio_h
#define cy_gpio_h

#include "pins.h"
#include <stdint.h>

enum PinModes
{
    CY_PINS_DM_AN_HIZ,
    CY_PINS_DM_DIG_HIZ,
    CY_PINS_DM_RES_UP,
    CY_PINS_DM_RES_DWN,
    CY_PINS_DM_OD_LO,
    CY_PINS_DM_OD_HI,
    CY_PINS_DM_STRONG,
    CY_PINS_DM_RES_UPDWN
};

struct cy_pin
{
    uint8_t val;
    uint8_t mode;
    uint8_t bypass;
};

void CyPins_SetPin(int pin);
void CyPins_ClearPin(int pin);
int  CyPins_ReadPin(int pin);

void CyPins_SetPinDriveMode(int pin, int mode);
int  CyPins_GetPinDriveMode(int pin);

#endif /* cy_gpio_h */
