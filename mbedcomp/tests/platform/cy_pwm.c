//
//  cy_pwm.c
//  tests
//
//  Created by Kristoffer Lyder Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "cy_pwm.h"

uint8_t PWM_started = 0;
uint8_t PWM_DC1 = 0;
uint8_t PWM_DC2 = 0;

void PWM_Start()
{
    PWM_started = 1;
    PWM_DC1 = 0;
    PWM_DC2 = 0;
}

void PWM_Stop()
{
    PWM_started = 0;
}

void PWM_WriteCompare1(uint8_t dc)
{
    PWM_DC1 = dc;
}

void PWM_WriteCompare2(uint8_t dc)
{
    PWM_DC2 = dc;
}

uint8_t PWM_ReadCompare1()
{
    return PWM_DC1;
}

uint8_t PWM_ReadCompare2()
{
    return PWM_DC2;
}