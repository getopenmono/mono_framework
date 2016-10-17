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
#include "mbed_assert.h"
#include "gpio_api.h"


uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    
    return 0;
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

}

void gpio_mode(gpio_t *obj, PinMode mode) {
    switch (mode) {
        case PullUp:
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_RES_UP);
            if (obj->dir == PIN_INPUT)
                gpio_write(obj, 1);
            break;
        case PullDown:
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_RES_DWN);
            if (obj->dir == PIN_INPUT)
                gpio_write(obj, 0);
            break;
        case OpenDrain:
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_OD_LO);
            break;
        case PullNone:
            if (obj->dir == PIN_INPUT)
                CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_DIG_HIZ);
            else
                CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_STRONG);
            break;
        default:
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_RES_DWN);
            break;
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    obj->dir = direction;
    
    switch (direction) {
        case PIN_INPUT :
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_DIG_HIZ);
            break;
        case PIN_OUTPUT:
            CyPins_SetPinDriveMode(obj->pin, CY_PINS_DM_STRONG);
            break;
    }
}
