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
#include <stddef.h>

#include "gpio_irq_api.h"
#include <mbed_debug.h>
#include <mbed_error.h>
#include <stdio.h>

#define CHANNEL_COUNT (9*8) // 9 ports with 8 pins each
#define PICU_COUNT 9

static uint32_t channel_ids[CHANNEL_COUNT] = {0};
static gpio_irq_handler irq_handler;


static void handle_interrupt_in(void) {

    uint8_t picus[PICU_COUNT] =
    {
        CY_GET_REG8(CYREG_PICU0_INTSTAT),
        CY_GET_REG8(CYREG_PICU1_INTSTAT),
        CY_GET_REG8(CYREG_PICU2_INTSTAT),
        CY_GET_REG8(CYREG_PICU3_INTSTAT),
        CY_GET_REG8(CYREG_PICU4_INTSTAT),
        CY_GET_REG8(CYREG_PICU5_INTSTAT),
        CY_GET_REG8(CYREG_PICU6_INTSTAT),
        CY_GET_REG8(CYREG_PICU12_INTSTAT),
        CY_GET_REG8(CYREG_PICU15_INTSTAT)
    };

    uint8_t bitloc;

    for (uint8_t p=0; p<PICU_COUNT;p++)
    {
        while (picus[p] > 0) {
            // get the position of the MSB in INTSTAT register
            bitloc = 31 - __CLZ(picus[p]);

            //test to see if there is an installed interrupt
            if (channel_ids[p*8+bitloc] != 0)
            {
                uint8_t type = 0;
                if (p < 7)
                    type = CY_GET_REG8( CYDEV_PICU_INTTYPE_BASE + (p*8+bitloc) );
                else if (p == 7)
                    type = CY_GET_REG8( CYDEV_PICU_INTTYPE_BASE + (12*8+bitloc) );
                else if (p == 8)
                    type = CY_GET_REG8( CYDEV_PICU_INTTYPE_BASE + (15*8+bitloc) );

                if (type == 1)
                    irq_handler(channel_ids[p*8+bitloc], IRQ_RISE);
                else if (type > 1) //also catches BOTH (change trigger)
                    irq_handler(channel_ids[p*8+bitloc], IRQ_FALL);
            }

            picus[p] -= 1<<bitloc;
        }
    }
}


int  gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) return -1;

    irq_handler = handler;
    obj->pinReg = pin;

    //find PORT for pin
    if      (CYDEV_IO_PC_PRT0_BASE == (pin & ~0x07))
    {
        obj->portNum = 0;
        obj->irqLine = PICU0_IRQ_LINE;
        obj->picuStat = CYREG_PICU0_INTSTAT;
        obj->snapShotAddr = CYREG_PICU0_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU0_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT1_BASE == (pin & ~0x07))
    {
        obj->portNum = 1;
        obj->irqLine = PICU1_IRQ_LINE;
        obj->picuStat = CYREG_PICU1_INTSTAT;
        obj->snapShotAddr = CYREG_PICU1_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU1_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT2_BASE == (pin & ~0x07))
    {
        obj->portNum = 2;
        obj->irqLine = PICU2_IRQ_LINE;
        obj->picuStat = CYREG_PICU2_INTSTAT;
        obj->snapShotAddr = CYREG_PICU2_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU2_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT3_BASE == (pin & ~0x07))
    {
        obj->portNum = 3;
        obj->irqLine = PICU3_IRQ_LINE;
        obj->picuStat = CYREG_PICU3_INTSTAT;
        obj->snapShotAddr = CYREG_PICU3_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU3_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT4_BASE == (pin & ~0x07))
    {
        obj->portNum = 4;
        obj->irqLine = PICU4_IRQ_LINE;
        obj->picuStat = CYREG_PICU4_INTSTAT;
        obj->snapShotAddr = CYREG_PICU4_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU4_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT5_BASE == (pin & ~0x07))
    {
        obj->portNum = 5;
        obj->irqLine = PICU5_IRQ_LINE;
        obj->picuStat = CYREG_PICU5_INTSTAT;
        obj->snapShotAddr = CYREG_PICU5_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU5_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT6_BASE == (pin & ~0x07))
    {
        obj->portNum = 6;
        obj->irqLine = PICU6_IRQ_LINE;
        obj->picuStat = CYREG_PICU6_INTSTAT;
        obj->snapShotAddr = CYREG_PICU6_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU6_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT12_BASE == (pin & ~0x07))
    {
        obj->portNum = 12;
        obj->irqLine = PICU12_IRQ_LINE;
        obj->picuStat = CYREG_PICU12_INTSTAT;
        obj->snapShotAddr = CYREG_PICU12_SNAP;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU12_BASE + (pin & 0x07);
    }
    else if (CYDEV_IO_PC_PRT15_BASE == (pin & ~0x07))
    {
        obj->portNum = 15;
        obj->irqLine = PICU15_IRQ_LINE;
        obj->picuStat = CYREG_PICU15_INTSTAT;
        obj->snapShotAddr = CYREG_PICU_15_SNAP_15;
        obj->inttypeReg = CYDEV_PICU_INTTYPE_PICU15_BASE + (pin & 0x07);
    }
    else
    {
        error("Interrupt PORT input not recognized for pin !\r\n");
    }

    // route the interrupt line to the fixed function component (which is PICU)
    uint32_t idmuxAddr = CYDEV_IDMUX_BASE + (obj->irqLine)/4; // 4 IDMUXes per register
    uint8_t idmuxVal = ~( 0x03 << ((obj->irqLine % 4)*2) );
    CY_SET_REG8(idmuxAddr, CY_GET_REG8(idmuxAddr) & idmuxVal);

    int index;

    // calc the interrupt table index
    if      (obj->portNum == 15)
        index = 8*8 + (pin & 0x07);
    else if (obj->portNum == 12)
        index = 7*8 + (pin & 0x07);
    else
        index = obj->portNum*8 + (pin & 0x07);

    // put us in the interrupt table
    channel_ids[index] = id;
    obj->ch = index;

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {

    if (!enable)
        CY_SET_REG8(obj->inttypeReg, 0x0);
    else if (event == IRQ_RISE)
        CY_SET_REG8(obj->inttypeReg, 0x01);
    else if (event == IRQ_FALL)
        CY_SET_REG8(obj->inttypeReg, 0x02);
    else if (event == (IRQ_FALL | IRQ_RISE))
        CY_SET_REG8(obj->inttypeReg, 0x03);
    else
        debug("Did not set any interrupt handler type!\r\n");

    obj->intTypeValue = CY_GET_REG8(obj->inttypeReg);
    CyIntSetVector(obj->irqLine, handle_interrupt_in);
    CyIntEnable(obj->irqLine);
}

void gpio_irq_enable(gpio_irq_t *obj) {
    CY_SET_REG8(obj->inttypeReg, obj->intTypeValue);
}

void gpio_irq_disable(gpio_irq_t *obj) {
     CY_SET_REG8(obj->inttypeReg, 0x0);
}
