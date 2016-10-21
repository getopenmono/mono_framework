/* OpenMono PSoC5 mbed Microcontroller Library
 * Copyright (c) 2015 Monolit ApS
 *
 * **NOTE** 
 * mbed uses a free run timer with a compare register interrupt to
 * implement ticker and wait api functions.
 * Cypress MCU timers can not be free run mode and compare mode at the same time
 * - therefore the implementation of the ticker_api on cypress is a bit cumbersome.
 * PSoC5 timers share the count and compare register!
 */
#include <stddef.h>
#include "us_ticker_api.h"
#include <project.h>


int us_ticker_inited = 0;

/** 
 * Global variable used by the ticker api, to hold current timer value
 * (timer count register substitute)
 * It needs to count to 2^32 us, which is 2^32 * 66 Mhz clock, therefore we use a
 * 64 bit integer, which is more than enough.
 */
uint64_t us_ticker_systick_timer_offset = 0;

/**
 * mbed expects the timer to count to 2^32, but Cypress timers are only 16 and
 * the SysTick Timer is 24 bit. This global variable is used to compensate
 * for the missing 8 bits of the timer.
 */
uint32_t us_ticker_systick_timer_remain_period = 0;

char us_ticker_systick_timer_interrupt_enabled = 0;

///**
// * The real 32 bit timer period for the software impl 32 bit timer needed by
// * mbed.
// */
//uint32_t us_ticker_systick_timer_real_period = 0;

void us_ticker_systick_irq_handler()
{
    //Counter reached 0 - add full count to global offset
    us_ticker_systick_timer_offset += CySysTickGetReload();
    
    if (us_ticker_systick_timer_remain_period == 0 && us_ticker_systick_timer_interrupt_enabled)
    {
        us_ticker_irq_handler();
    }
    else if (us_ticker_systick_timer_remain_period > 0)
    {
        if (us_ticker_systick_timer_remain_period > 0xFFFFFF)
        {
            CySysTickSetReload(0xFFFFFF);
            us_ticker_systick_timer_remain_period = us_ticker_systick_timer_remain_period - 0xFFFFFF;
        }
        else
        {
            CySysTickSetReload(us_ticker_systick_timer_remain_period);
            us_ticker_systick_timer_remain_period = 0;
        }
        //Reset timer
        CySysTickClear();
    }
}

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
    
    CySysTickStart();
    CyIntSetSysVector(CY_INT_SYSTICK_IRQN, &us_ticker_systick_irq_handler);
    
    CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK); // ticks with system master clock clock
    CySysTickSetReload(0xFFFFFF); // maximum timeout is 2^24
    CySysTickClear();
    CyGlobalIntEnable;
}

uint32_t us_ticker_read() {
    if (!us_ticker_inited)
        us_ticker_init();
    
	return (us_ticker_systick_timer_offset + (CySysTickGetReload() - CySysTickGetValue())) / BCLK__BUS_CLK__MHZ; // get number of 1 us ticks
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    CySysTickDisableInterrupt();
    uint32_t now = us_ticker_read();
    uint32_t utime = timestamp;

    if (now >= utime)
    {
        // Systick cannot be negative or zero
        // if systick is zero it will be disabled
        now = utime - 10;
    }

    // diff is in clock cycles
    uint64_t diff = (utime - now)*(uint64_t)BCLK__BUS_CLK__MHZ;
    
    us_ticker_systick_timer_offset += CySysTickGetReload() - CySysTickGetValue();
    
    if (diff > 0xFFFFFF)
    {
        CySysTickSetReload(0xFFFFFF);
        us_ticker_systick_timer_remain_period = diff - 0xFFFFFF;
    }
    else
    {
        CySysTickSetReload(diff);
        us_ticker_systick_timer_remain_period = 0;
    }
    
    CySysTickClear();
    us_ticker_systick_timer_interrupt_enabled = 1;
    CySysTickEnableInterrupt();
}

void us_ticker_disable_interrupt(void) {
    us_ticker_systick_timer_interrupt_enabled = 0;
    //count to 0xFFFFFF to support us_ticker_read() getting larger and larger
    CySysTickSetReload(0xFFFFFF); // maximum timeout is 2^24
}

void us_ticker_clear_interrupt(void) {
    
}
