// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include <project.h>
#include <stdbool.h>
#include <rtc_api.h>

static bool _rtcenabled = false;
time_t _mbed_rtc_systemtime = 0; //compiled default is EPOCH

void _rtc_interrupt()
{
    /* Clear OPPS interrupt status flag */
    CyPmReadStatus(CY_PM_ONEPPS_INT);
    
    _mbed_rtc_systemtime++;
}

void rtc_init(void)
{
    //disable rtc interrupt line
    CyIntDisable(RTC_isr__INTC_NUMBER);
    
    //Setup mcu registers and interrupts
    /* Set the ISR to point to the RTC_SUT_isr Interrupt. */
    CyIntSetVector(RTC_isr__INTC_NUMBER, &_rtc_interrupt);
    
    /* Set the priority. */
    CyIntSetPriority(RTC_isr__INTC_NUMBER, RTC_isr__INTC_PRIOR_NUM);
    
    uint8 interruptState;
    
    /* Enter critical section */
    interruptState = CyEnterCriticalSection();
    
    /* Enable one pulse per second event and interrupt */
    uint8_t regContent = CY_GET_REG8(CYREG_PM_TW_CFG2);
    CY_SET_REG8(CYREG_PM_TW_CFG2, regContent | (RTC_OPPS_EN_MASK | RTC_OPPSIE_EN_MASK));
    
    /* Exit critical section */
    CyExitCriticalSection(interruptState);
    
    /* Enable the interrupt */
    CyIntEnable(RTC_ISR_NUMBER);
    
    _rtcenabled = true;
}

void rtc_free(void)
{
    /* Disable the interrupt. */
    CyIntDisable(RTC_ISR_NUMBER);
    
    _rtcenabled = false;
}

int rtc_isenabled(void)
{
    return _rtcenabled;
}

time_t rtc_read(void)
{
    return _mbed_rtc_systemtime;
}

void rtc_write(time_t t)
{
    _mbed_rtc_systemtime = t;
}
