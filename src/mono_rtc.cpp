// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

//#include <project.h>
#include "mono_rtc.h"
#include "date_time.h"
#include "mono_power_management.h"
#include <rtc_api.h>

extern "C" time_t _mbed_rtc_systemtime;

using namespace mono;

bool IRTCSystem::__shouldProcessScheduledTasks = false;

extern "C" {
void rtc_interrupt(void)
{
    /* Clear OPPS interrupt status flag */
    CyPmReadStatus(CY_PM_ONEPPS_INT);

    _mbed_rtc_systemtime++;
    DateTime::incrementSystemClock();
    IRTCSystem::__shouldProcessScheduledTasks = true;
}
}

void MonoRTC::setupRtcSystem()
{
    if (!rtc_isenabled())
        rtc_init();
    
    CyIntDisable(RTC_ISR_NUMBER);
    
    //Setup mcu registers and interrupts
    /* Set the ISR to point to the RTC_SUT_isr Interrupt. */
    CyIntSetVector(RTC_isr__INTC_NUMBER, &rtc_interrupt);
    
//    //disable rtc interrupt line
//    CyIntDisable(RTC_isr__INTC_NUMBER);
//
//    //Setup mcu registers and interrupts
//    /* Set the ISR to point to the RTC_SUT_isr Interrupt. */
//    CyIntSetVector(RTC_isr__INTC_NUMBER, &rtc_interrupt);
//
//    /* Set the priority. */
//    CyIntSetPriority(RTC_isr__INTC_NUMBER, RTC_isr__INTC_PRIOR_NUM);
//
//    uint8 interruptState;
//
//    /* Enter critical section */
//    interruptState = CyEnterCriticalSection();
//    
//    /* Enable one pulse per second event and interrupt */
//    uint8_t regContent = CY_GET_REG8(CYREG_PM_TW_CFG2);
//    CY_SET_REG8(CYREG_PM_TW_CFG2, regContent | (RTC_OPPS_EN_MASK | RTC_OPPSIE_EN_MASK));
//
//    /* Exit critical section */
//    CyExitCriticalSection(interruptState);

}

void MonoRTC::startRtc()
{
    /* Enable the interrupt */
    CyIntEnable(RTC_ISR_NUMBER);
}

void MonoRTC::stopRtc()
{
    /* Disable the interrupt. */
    CyIntDisable(RTC_ISR_NUMBER);
}
