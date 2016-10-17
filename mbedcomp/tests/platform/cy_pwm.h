//
//  cy_pwm.h
//  tests
//
//  Created by Kristoffer Lyder Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef cy_pwm_h
#define cy_pwm_h

#include <stdint.h>

void PWM_Start();
void PWM_Stop();

void PWM_WriteCompare1(uint8_t dc);
void PWM_WriteCompare2(uint8_t dc);

uint8_t PWM_ReadCompare1();
uint8_t PWM_ReadCompare2();

#endif /* cy_pwm_h */
