/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "gpio_api.h"
#include "wait_api.h"
#include "toolchain.h"
#include "mbed_interface.h"
#include "PinNames.h"

WEAK void mbed_die(void) {
    
#ifndef EMUNO
    CyHalt(1);
    PWM_Start();
    PWM_WriteCompare2(0);
    CyPins_SetPinDriveMode(SW_USER, CY_PINS_DM_RES_UP);
    Bootloadable_SET_RUN_TYPE(0); // do not force bootloader
    
    while(1)
    {
        PWM_WriteCompare1(16);
        CyDelay(250);
        PWM_WriteCompare1(64);
        
        if (CyPins_ReadPin(SW_USER) == 0)
        {
            CyDelay(200); // allow yser to release btn, to not go into bootloader
            CySoftwareReset();
        }
        
        CyDelay(400);
        
        if (CyPins_ReadPin(SW_USER) == 0)
        {
            CyDelay(200); // allow yser to release btn, to not go into bootloader
            CySoftwareReset();
        }
    }
#endif

}
