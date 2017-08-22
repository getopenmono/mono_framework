/*******************************************************************************
* File Name: RTC.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the RTC Component.
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "RTC.h"
#include "CyLib.h"

/* Function Prototypes */
static void  RTC_SetInitValues(void)     ;
static uint8 RTC_DayOfWeek(uint8 dayOfMonth, uint8 month, uint16 year)
                                                      ;
#if (1u == RTC_DST_FUNC_ENABLE)
    static void  RTC_DSTDateConversion(void) ;
#endif /* 1u == RTC_DST_FUNC_ENABLE */


/* Variables were not initialized */
uint8 RTC_initVar = 0u;

/* Time and date variables
* Initial value are: Second = 0-59, Minute = 0-59, Hour = 0-23, DayOfWeek = 1-7,
* DayOfMonth = 1-31, DayOfYear = 1-366, Month = 1-12, Year = 1900-2200.
*/
RTC_TIME_DATE RTC_currentTimeDate = {0u, 0u, 0u, 1u, 1u, 1u, 1u, 1900u};

/* Alarm time and date variables
* Initial value are: Second = 0-59, Minute = 0-59, Hour = 0-23, DayOfWeek = 1-7,
* DayOfMonth = 1-31, DayOfYear = 1-366, Month = 1-12, Year = 1900-2200.
*/
RTC_TIME_DATE RTC_alarmCfgTimeDate = {0u, 0u, 0u, 1u, 1u, 1u, 1u, 1900u};

#if (1u == RTC_DST_FUNC_ENABLE) /* DST enabled */

    /* Define DST format: '0' - fixed, '1' - relative */
    volatile uint8 RTC_dstModeType = 0u;

    /* Hour 0-23, DayOfWeek 1-7, Week 1-5, DayOfMonth 1-31, Month 1-12 */
    RTC_DSTIME RTC_dstTimeDateStart = {0u, 1u, 1u, 1u, 1u};
    RTC_DSTIME RTC_dstTimeDateStop = {0u, 1u, 1u, 1u, 1u};

    /* Number of Hours to add/dec to time */
    volatile uint8 RTC_dstOffsetMin = 0u;
    volatile uint8 RTC_dstStartStatus = 0u;
    volatile uint8 RTC_dstStopStatus = 0u;

#endif /* 1u == RTC_DST_FUNC_ENABLE*/

/* Mask Registers */
volatile uint8 RTC_alarmCfgMask = 0u;
volatile uint8 RTC_alarmCurStatus = 0u;
volatile uint8 RTC_intervalCfgMask = 0u;

/* Status & Control Variables */
volatile uint8 RTC_statusDateTime = 0u;

/* Month Day Array - number of days in the months */
const uint8 CYCODE RTC_daysInMonths[RTC_MONTHS_IN_YEAR] = {
    RTC_DAYS_IN_JANUARY,
    RTC_DAYS_IN_FEBRUARY,
    RTC_DAYS_IN_MARCH,
    RTC_DAYS_IN_APRIL,
    RTC_DAYS_IN_MAY,
    RTC_DAYS_IN_JUNE,
    RTC_DAYS_IN_JULY,
    RTC_DAYS_IN_AUGUST,
    RTC_DAYS_IN_SEPTEMBER,
    RTC_DAYS_IN_OCTOBER,
    RTC_DAYS_IN_NOVEMBER,
    RTC_DAYS_IN_DECEMBER};


/*******************************************************************************
* Function Name:   RTC_Start
********************************************************************************
*
* Summary:
*  Enables RTC component: configures counter, setup interrupts, done all
*  required calculation and starts counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  RTC_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time RTC_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the RTC_Start() routine.
*
*  RTC_currentTimeDate, RTC_dstTimeDateStart,
*  RTC_dstTimeDateStop, RTC_dstTimeDateStart,
*  RTC_alarmCfgTimeDate, RTC_statusDateTime,
*  RTC_dstStartStatus, RTC_dstStopStatus,
*  RTC_alarmCurStatus: global variables are modified by the
*  functions called from RTC_Init().
*
* Reentrant:
*  No.
*
* Side Effects:
*  Enables for the one pulse per second (for the RTC component) and
*  Central Time Wheel (for the Sleep Timer component) signals to wake up device
*  from the low power (Sleep and Alternate Active) modes and leaves them
*  enabled.
*
*  The Power Manager API has the higher priority on resource usage: it is NOT
*  guaranteed that the Sleep Timer's configuration will be the same on exit
*  from the Power Manager APIs as on the entry. To prevent this use the Sleep
*  Timer's Sleep() - to save configuration and stop the component and Wakeup()
*  function to restore configuration and enable the component.
*
*  The Sleep Timer and Real Time Clock (RTC) components could be configured as
*  a wake up source from the low power modes only both at once.
*
*******************************************************************************/
void RTC_Start(void) 
{
    /* Execute once in normal flow */
    if(0u == RTC_initVar)
    {
        RTC_Init();
        RTC_initVar = 1u;
    }

    /* Enable component's operation */
    RTC_Enable();
}


/*******************************************************************************
* Function Name: RTC_Stop
********************************************************************************
*
* Summary:
*  Stops the RTC component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Leaves the one pulse per second (for the RTC component) and the Central Time
*  Wheel (for the Sleep Timer component) signals to wake up device from the low
*  power (Sleep and Alternate Active) modes enabled after Sleep Time component
*  is stopped.
*
*******************************************************************************/
void RTC_Stop(void) 
{
    uint8 interruptState;

    /* Disable the interrupt. */
    CyIntDisable(RTC_ISR_NUMBER);

    /* Enter critical section */
    interruptState = CyEnterCriticalSection();

    /* Stop one pulse per second counter and interrupt */
    RTC_OPPS_CFG_REG &= (uint8)(~(RTC_OPPSIE_EN_MASK | RTC_OPPS_EN_MASK));

    /* Exit critical section */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name:   RTC_EnableInt
********************************************************************************
*
* Summary:
*  Enables interrupts of RTC Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void RTC_EnableInt(void) 
{
    /* Enable the interrupt */
    CyIntEnable(RTC_ISR_NUMBER);
}


/*******************************************************************************
* Function Name:   RTC_DisableInt
********************************************************************************
*
* Summary:
*  Disables interrupts of RTC Component, time and date stop running.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void RTC_DisableInt(void) 
{
    /* Disable the interrupt. */
    CyIntDisable(RTC_ISR_NUMBER);
}


#if (1u == RTC_DST_FUNC_ENABLE)
    /*******************************************************************************
    * Function Name:   RTC_DSTDateConversion
    ********************************************************************************
    *
    * Summary:
    * Converts relative to absolute date.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week,
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Week,
    *  RTC_currentTimeDate.Year: these global variables are
    *  used to correct day of week.
    *
    *  RTC_dstTimeDateStart.DayOfMonth,
    *  RTC_dstTimeDateStop.DayOfMonth: these global variables are
    *  modified after convertion.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    static void RTC_DSTDateConversion(void) 
    {
        uint8 week = 1u;
        uint8 day = 1u;
        uint8 dayOfWeek;

        /* Get day of week */
        dayOfWeek = RTC_DayOfWeek(day, RTC_dstTimeDateStart.Month,
                                                    RTC_currentTimeDate.Year) + 1u;

        #if (0u != RTC_START_OF_WEEK)
        /* Normalize day of week if Start of week is not Sunday */
        if(dayOfWeek > RTC_START_OF_WEEK)
        {
            #if (6u != RTC_START_OF_WEEK)
                /* Start of week is not Saturday  */
                dayOfWeek -= RTC_START_OF_WEEK;
            #else /* 6u == RTC_START_OF_WEEK */
                /* Start of week is Saturday  */
                dayOfWeek = 1u; /* Set day of week to Monday */
            #endif /* 6u != RTC_START_OF_WEEK */
        }
        else
        {
            #if (1u != RTC_START_OF_WEEK)
                /* Start of week is not Monday  */
                dayOfWeek = (RTC_DAYS_IN_WEEK - RTC_START_OF_WEEK) - dayOfWeek;
            #else /* 1u == RTC_START_OF_WEEK */
                /* Start of week is Monday  */
                dayOfWeek = 5u; /* Set day of week to Friday */
            #endif /* 1u != RTC_START_OF_WEEK */
        }
        #endif /* 0u != RTC_START_OF_WEEK */

        /* Correct if out of DST range */
        while(dayOfWeek != RTC_dstTimeDateStart.DayOfWeek)
        {
            day++;
            dayOfWeek++;
            if(dayOfWeek > RTC_WEEK_ELAPSED)
            {
                dayOfWeek = 1u;
                week++;
            }
        }

        while(week != RTC_dstTimeDateStart.Week)
        {
            day += RTC_DAYS_IN_WEEK;
            week++;
        }
        RTC_dstTimeDateStart.DayOfMonth = day;

        /* Stop of DST time */
        week = 1u;
        day = 1u;

        dayOfWeek = RTC_DayOfWeek(day, RTC_dstTimeDateStop.Month,
                                                    RTC_currentTimeDate.Year) + 1u;
        #if (0u != RTC_START_OF_WEEK)
        /* Normalize day of week if Start of week is not Sunday */
        if(dayOfWeek > RTC_START_OF_WEEK)
        {
            #if (6u != RTC_START_OF_WEEK)
                /* Start of week is not Saturday  */
                dayOfWeek -= RTC_START_OF_WEEK;
            #else /* 6u == RTC_START_OF_WEEK */
                /* Start of week is Saturday  */
                dayOfWeek = 1u; /* Set day of week to Monday */
            #endif /* 6u != RTC_START_OF_WEEK */
        }
        else
        {
            #if (1u != RTC_START_OF_WEEK)
                /* Start of week is not Monday  */
                dayOfWeek = (RTC_DAYS_IN_WEEK - RTC_START_OF_WEEK) - dayOfWeek;
            #else /* 1u == RTC_START_OF_WEEK */
                /* Start of week is Monday  */
                dayOfWeek = 5u; /* Set day of week to Friday */
            #endif /* 1u != RTC_START_OF_WEEK */
        }
        #endif /* 0u != RTC_START_OF_WEEK */

        while(dayOfWeek != RTC_dstTimeDateStop.DayOfWeek)
        {
            day++;
            dayOfWeek++;
            if(dayOfWeek > RTC_WEEK_ELAPSED)
            {
                dayOfWeek = 1u;
                week++;
            }
        }

        while(week != RTC_dstTimeDateStop.Week)
        {
            day += RTC_DAYS_IN_WEEK;
            week++;
        }

        RTC_dstTimeDateStop.DayOfMonth = day;
    }
#endif /* 1u == RTC_DST_FUNC_ENABLE */


/*******************************************************************************
* Function Name:   RTC_Init
********************************************************************************
*
* Summary:
*  Calculates required date and flags, sets interrupt vector and priority.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate, RTC_dstTimeDateStart,
*  RTC_dstTimeDateStop, RTC_dstTimeDateStart,
*  RTC_alarmCfgTimeDate, RTC_statusDateTime,
*  RTC_dstStartStatus, RTC_dstStartStatus,
*  RTC_dstStopStatus, RTC_alarmCurStatus:
*  global variables are used by the RTC_SetInitValues().
*
*  RTC_dstTimeDateStart, RTC_currentTimeDate:
*  RTC_statusDateTime, RTC_dstStartStatus,
*  RTC_dstStartStatus, RTC_dstStopStatus,
*  RTC_alarmCurStatus: are modified by the
*  RTC_SetInitValues() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_Init(void) 
{
    /* Start calculation of required date and flags */
    RTC_SetInitValues();

    /* Disable Interrupt. */
    CyIntDisable(RTC_ISR_NUMBER);

    /* Set the ISR to point to the RTC_SUT_isr Interrupt. */
    (void) CyIntSetVector(RTC_ISR_NUMBER, & RTC_ISR);

    /* Set the priority. */
    CyIntSetPriority(RTC_ISR_NUMBER, RTC_ISR_PRIORITY);
}


/*******************************************************************************
* Function Name: RTC_Enable
********************************************************************************
*
* Summary:
*  Enables the interrupts, one pulse per second and interrupt generation on OPPS
*  event.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Enables for the one pulse per second and central time wheel signals to wake
*  up device from the low power (Sleep and Alternate Active) modes and leaves
*  them enabled.
*
*******************************************************************************/
void RTC_Enable(void) 
{
    uint8 interruptState;

    /* Enter critical section */
    interruptState = CyEnterCriticalSection();

    /* Enable one pulse per second event and interrupt */
    RTC_OPPS_CFG_REG |= (RTC_OPPS_EN_MASK | RTC_OPPSIE_EN_MASK);

    /* Exit critical section */
    CyExitCriticalSection(interruptState);

    /* Enable interrupt */
    CyIntEnable(RTC_ISR_NUMBER);
}


/*******************************************************************************
* Function Name:   RTC_ReadTime
********************************************************************************
*
* Summary:
*  Returns a pointer to the current time and date structure.
*
* Parameters:
*  None.
*
* Return:
*  RTC_currentTimeDate: pointer to the global structure with the
*  current date and time values.
*
* Global variables:
*  RTC_currentTimeDate: global variable with current date and
*   time is used.
*
* Side Effects:
*  You should disable the interrupt for the RTC component before calling any
*  read API to avoid an RTC Counter increment in the middle of a time or date
*  read operation. Re-enable the interrupts after the data is read.
*
*******************************************************************************/
RTC_TIME_DATE * RTC_ReadTime(void) 
{
    /* Returns a pointer to the current time and date structure */
    return (&RTC_currentTimeDate);
}


/*******************************************************************************
* Function Name:   RTC_WriteTime
********************************************************************************
*
* Summary:
*  Writes time and date values as current time and date. Only
*  passes Milliseconds(optionally), Seconds, Minutes, Hours, Month,
*  Day Of Month and Year.
*
* Parameters:
*  timeDate: Pointer to RTC_TIME_DATE global structure where new
*  values of time and date are stored.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate: global structure is modified with the new
*  values of current date and time.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteTime(const RTC_TIME_DATE * timeDate)
     
{
    /* Disable Interrupt of RTC Component */
    RTC_DisableInt();

    /* Write current time and date */
    RTC_currentTimeDate.Sec = timeDate->Sec;
    RTC_currentTimeDate.Min = timeDate->Min;
    RTC_currentTimeDate.Hour = timeDate->Hour;
    RTC_currentTimeDate.DayOfMonth = timeDate->DayOfMonth;
    RTC_currentTimeDate.Month = timeDate->Month;
    RTC_currentTimeDate.Year = timeDate->Year;

    /* Enable Interrupt of RTC Component */
    RTC_EnableInt();
}


/*******************************************************************************
* Function Name:   RTC_WriteSecond
********************************************************************************
*
* Summary:
*  Writes Sec software register value.
*
* Parameters:
*  second: Seconds value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Sec: global structure's field where current
*  second's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteSecond(uint8 second) 
{
    /* Save seconds to the current time and date structure */
    RTC_currentTimeDate.Sec = second;
}


/*******************************************************************************
* Function Name:   RTC_WriteMinute
********************************************************************************
*
* Summary:
*  Writes Minute value in minutes counter register.
*
* Parameters:
*  minute: Minutes value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Min: global structure's field where
*  current minute's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteMinute(uint8 minute) 
{
    /* Save minutes to the current time and date structure */
    RTC_currentTimeDate.Min = minute;
}


/*******************************************************************************
* Function Name:   RTC_WriteHour
********************************************************************************
*
* Summary:
*  Writes Hour software register value.
*
* Parameters:
*  hour: Hours value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Hour: global structure's field where
*  current hour's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteHour(uint8 hour) 
{
    /* Save hours to the current time and date structure */
    RTC_currentTimeDate.Hour = hour;
}


/*******************************************************************************
* Function Name:   RTC_WriteDayOfMonth
********************************************************************************
*
* Summary:
*  Writes DayOfMonth software register value.
*
* Parameters:
*  dayOfMonth: Day Of Month value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.DayOfMonth: global structure's field where
*  current day of month's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteDayOfMonth(uint8 dayOfMonth) 
{
    /* Save day of month to the current time and date structure */
    RTC_currentTimeDate.DayOfMonth = dayOfMonth;
}


/*******************************************************************************
* Function Name:   RTC_WriteMonth
********************************************************************************
*
* Summary:
*  Writes Month software register value.
*
* Parameters:
*  month: Month value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Month: global structure's field where
*  current day of month's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteMonth(uint8 month) 
{
    /* Save months to the current time and date structure */
    RTC_currentTimeDate.Month = month;
}


/*******************************************************************************
* Function Name:   RTC_WriteYear
********************************************************************************
*
* Summary:
*  Writes Year software register value.
*
* Parameters:
*  year: Years value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Year: global structure's field where
*  current year's value is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteYear(uint16 year) 
{
    /* Save years to the current time and date structure */
    RTC_currentTimeDate.Year = year;
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmSecond
********************************************************************************
*
* Summary:
*  Writes Alarm Sec software register value.
*
* Parameters:
*  second: Alarm Seconds value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Sec: this global variable is used for
*  comparison while setting and clearing seconds alarm status variable.
*
*  RTC_alarmCfgTimeDate.Sec: this global variable is modified to
*  store of the new seconds alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  second's alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmSecond(uint8 second) 
{
    RTC_alarmCfgTimeDate.Sec = second;

    /* Check second alarm */
    if(RTC_alarmCfgTimeDate.Sec == RTC_currentTimeDate.Sec)
    {
        /* Set second alarm */
        RTC_alarmCurStatus |= RTC_ALARM_SEC_MASK;
    }
    else /* no second alarm */
    {
        /* Clear second alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_SEC_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmMinute
********************************************************************************
*
* Summary:
*  Writes Alarm Min software register value.
*
* Parameters:
*  minute: Alarm minutes value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Min: this global variable is used for
*  comparison while setting and clearing minutes alarm status variable.
*
*  RTC_alarmCfgTimeDate.Min: this global variable is modified to
*  store of the new minutes alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  minute's alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmMinute(uint8 minute) 
{
    RTC_alarmCfgTimeDate.Min = minute;

    /* Check minute alarm */
    if(RTC_alarmCfgTimeDate.Min == RTC_currentTimeDate.Min)
    {
        /* Set minute alarm */
        RTC_alarmCurStatus |= RTC_ALARM_MIN_MASK;
    }
    else /* no minute alarm */
    {
        /* Clear minute alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_MIN_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmHour
********************************************************************************
*
* Summary:
*  Writes Alarm Hour software register value.
*
* Parameters:
*  hour: Alarm hours value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Hour: this global variable is used for
*  comparison while setting and clearing hours alarm status variable.
*
*  RTC_alarmCfgTimeDate.Hour: this global variable is modified to
*  store of the new hours alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  hours alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmHour(uint8 hour) 
{
    RTC_alarmCfgTimeDate.Hour = hour;

    /* Check hour alarm */
    if(RTC_alarmCfgTimeDate.Hour == RTC_currentTimeDate.Hour)
    {
        /* Set hour alarm */
        RTC_alarmCurStatus |= RTC_ALARM_HOUR_MASK;
    }
    else /* no hour alarm */
    {
        /* Clear hour alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_HOUR_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmDayOfMonth
********************************************************************************
*
* Summary:
*  Writes Alarm DayOfMonth software register value.
*
* Parameters:
*  dayOfMonth: Alarm day of month value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.DayOfMonth: this global variable is used for
*  comparison while setting and clearing day of month alarm status variable.
*
*  RTC_alarmCfgTimeDate.DayOfMonth: this global variable is
*  modified to store of the new day of month alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  day of month alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmDayOfMonth(uint8 dayOfMonth) 
{
    RTC_alarmCfgTimeDate.DayOfMonth = dayOfMonth;

    /* Check day of month alarm */
    if(RTC_alarmCfgTimeDate.DayOfMonth == RTC_currentTimeDate.DayOfMonth)
    {
        /* Set day of month alarm */
        RTC_alarmCurStatus |= RTC_ALARM_DAYOFMONTH_MASK;
    }
    else /* no day of month alarm */
    {
        /* Clear day of month alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFMONTH_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmMonth
********************************************************************************
*
* Summary:
*  Writes Alarm Month software register value.
*
* Parameters:
*  month: Alarm month value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Month: this global variable is used for
*  comparison while setting and clearing month alarm status variable.
*
*  RTC_alarmCfgTimeDate.Month: this global variable is modified
*  to store of the new month alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  month alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmMonth(uint8 month) 
{
    RTC_alarmCfgTimeDate.Month = month;

    /* Check month alarm */
    if(RTC_alarmCfgTimeDate.Month == RTC_currentTimeDate.Month)
    {
        /* Set month alarm */
        RTC_alarmCurStatus |= RTC_ALARM_MONTH_MASK;
    }
    else /* no month alarm */
    {
        /* Clear month alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_MONTH_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmYear
********************************************************************************
*
* Summary:
*  Writes Alarm Year software register value.
*
* Parameters:
*  year: Alarm year value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.Year: this global variable is used for
*  comparison while setting and clearing year alarm status variable.
*
*  RTC_alarmCfgTimeDate.Year: this global variable is modified
*  to store of the new year alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  year alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmYear(uint16 year) 
{
   RTC_alarmCfgTimeDate.Year = year;

    /* Check year alarm */
    if(RTC_alarmCfgTimeDate.Year == RTC_currentTimeDate.Year)
    {
        /* Set year alarm */
        RTC_alarmCurStatus |= RTC_ALARM_YEAR_MASK;
    }
    else /* no year alarm */
    {
        /* Set year alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_YEAR_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmDayOfWeek
********************************************************************************
*
* Summary:
*   Writes Alarm DayOfWeek software register value.
*   Days values {Sun = 1, Mon = 2, Tue = 3, Wen = 4, Thu = 5, Fri = 6, Sat = 7}
*
* Parameters:
*  dayOfWeek: Alarm day of week value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.DayOfWeek: this global variable is used for
*  comparison while setting and clearing day of week alarm status variable.
*
*  RTC_alarmCfgTimeDate.DayOfWeek: this global variable is modified
*  to store of the new day of week alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  day of week alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmDayOfWeek(uint8 dayOfWeek) 
{
    RTC_alarmCfgTimeDate.DayOfWeek = dayOfWeek;

    /* Check day of week alarm */
    if(RTC_alarmCfgTimeDate.DayOfWeek == RTC_currentTimeDate.DayOfWeek)
    {
        /* Set day of week alarm */
        RTC_alarmCurStatus |= RTC_ALARM_DAYOFWEEK_MASK;
    }
    else /* no day of week alarm */
    {
        /* Set day of week alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFWEEK_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmDayOfYear
********************************************************************************
*
* Summary:
*  Writes Alarm DayOfYear software register value.
*
* Parameters:
*  dayOfYear: Alarm day of year value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate.DayOfYear: this global variable is used for
*  comparison while setting and clearing day of year alarm status variable.
*
*  RTC_alarmCfgTimeDate.DayOfYear: this global variable is modified
*  to store of the new day of year alarm.
*
*  RTC_alarmCurStatus: this global variable could be changed if
*  day of year alarm will be raised.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmDayOfYear(uint16 dayOfYear) 
{
  RTC_alarmCfgTimeDate.DayOfYear = dayOfYear;

    /* Check day of year alarm */
    if(RTC_alarmCfgTimeDate.DayOfYear == RTC_currentTimeDate.DayOfYear)
    {
        /* Set day of year alarm */
        RTC_alarmCurStatus |= RTC_ALARM_DAYOFYEAR_MASK;
    }
    else /* no day of year alarm */
    {
        /* Set day of year alarm */
        RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFYEAR_MASK);
    }
}


/*******************************************************************************
* Function Name:   RTC_ReadSecond
********************************************************************************
*
* Summary:
*  Reads Sec software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current seconds value.
*
* Global variables:
*  RTC_currentTimeDate.Sec: the current second's value is used.
*
*******************************************************************************/
uint8 RTC_ReadSecond(void) 
{
    /* Return current second */
    return (RTC_currentTimeDate.Sec);
}


/*******************************************************************************
* Function Name:   RTC_ReadMinute
********************************************************************************
*
* Summary:
*  Reads Min software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current field's value is returned.
*
* Global variables:
*  RTC_currentTimeDate.Min: the current field's value is used.
*
*******************************************************************************/
uint8 RTC_ReadMinute(void) 
{
    /* Return current minute */
    return (RTC_currentTimeDate.Min);
}


/*******************************************************************************
* Function Name:   RTC_ReadHour
********************************************************************************
*
* Summary:
*  Reads Hour software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current hour's value.
*
* Global variables:
*  RTC_currentTimeDate.Hour: the current field's value is used.
*
*******************************************************************************/
uint8 RTC_ReadHour(void) 
{
    /* Return current hour */
    return (RTC_currentTimeDate.Hour);
}


/*******************************************************************************
* Function Name:   RTC_ReadDayOfMonth
********************************************************************************
*
* Summary:
*  Reads DayOfMonth software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current value of the day of month.
*  returned.
*
* Global variables:
*  RTC_currentTimeDate.DayOfMonth: the current day of month's
*  value is used.
*
*******************************************************************************/
uint8 RTC_ReadDayOfMonth(void) 
{
    /* Return current day of the month */
    return (RTC_currentTimeDate.DayOfMonth);
}


/*******************************************************************************
* Function Name:   RTC_ReadMonth
********************************************************************************
*
* Summary:
*  Reads Month software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current value of the month.
*
* Global variables:
*  RTC_currentTimeDate.Month: the current month's value is used.
*
*******************************************************************************/
uint8 RTC_ReadMonth(void) 
{
    /* Return current month */
    return (RTC_currentTimeDate.Month);
}


/*******************************************************************************
* Function Name:   RTC_ReadYear
********************************************************************************
*
* Summary:
*  Reads Year software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current value of the year.
*
* Global variables:
*  RTC_currentTimeDate.Year: the current year's value is used.
*
*******************************************************************************/
uint16 RTC_ReadYear(void) 
{
    /* Return current year */
    return (RTC_currentTimeDate.Year);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmSecond
********************************************************************************
*
* Summary:
*  Reads Alarm Sec software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the seconds.
*
* Global variables:
*  RTC_alarmCfgTimeDate.Sec: the current second alarm value is
*  used.
*
********************************************************************************/
uint8 RTC_ReadAlarmSecond(void) 
{
    /* Return current alarm second */
    return (RTC_alarmCfgTimeDate.Sec);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmMinute
********************************************************************************
*
* Summary:
*  Reads Alarm Min software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the minutes.
*
* Global variables:
*  RTC_alarmCfgTimeDate.Min: the current minute alarm is used.
*
*******************************************************************************/
uint8 RTC_ReadAlarmMinute(void) 
{
    /* Return current alarm minute */
    return (RTC_alarmCfgTimeDate.Min);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmHour
********************************************************************************
*
* Summary:
*  Reads Alarm Hour software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the hours.
*
* Global variables:
*  RTC_alarmCfgTimeDate.Hour: the current hour alarm value is used.
*
*******************************************************************************/
uint8 RTC_ReadAlarmHour(void) 
{
    /* Return current alarm hour */
    return (RTC_alarmCfgTimeDate.Hour);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmDayOfMonth
********************************************************************************
*
* Summary:
*  Reads Alarm DayOfMonth software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the day of month.
*
* Global variables:
*  RTC_alarmCfgTimeDate.DayOfMonth: the current day of month alarm
*  value is used.
*
*******************************************************************************/
uint8 RTC_ReadAlarmDayOfMonth(void) 
{
    /* Return current alarm day of month */
    return (RTC_alarmCfgTimeDate.DayOfMonth);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmMonth
********************************************************************************
*
* Summary:
*  Reads Alarm Month software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the month.
*
* Global variables:
*  RTC_alarmCfgTimeDate.Month: the current month alarm value is
*  used.
*
*******************************************************************************/
uint8 RTC_ReadAlarmMonth(void) 
{
    /* Return current alarm month */
    return (RTC_alarmCfgTimeDate.Month);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmYear
********************************************************************************
*
* Summary:
*  Reads Alarm Year software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the years.
*
* Global variables:
*  RTC_alarmCfgTimeDate.Year: the current year alarm value is used.
*
*******************************************************************************/
uint16 RTC_ReadAlarmYear(void) 
{
    /* Return current alarm year */
    return (RTC_alarmCfgTimeDate.Year);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmDayOfWeek
********************************************************************************
*
* Summary:
*  Reads Alarm DayOfWeek software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the day of week.
*
* Global variables:
*  RTC_alarmCfgTimeDate.DayOfWeek: the current day of week alarm
*  value is used.
*
*******************************************************************************/
uint8 RTC_ReadAlarmDayOfWeek(void) 
{
    /* Return current alarm day of the week */
    return (RTC_alarmCfgTimeDate.DayOfWeek);
}


/*******************************************************************************
* Function Name:   RTC_ReadAlarmDayOfYear
********************************************************************************
*
* Summary:
*  Reads Alarm DayOfYear software register value.
*
* Parameters:
*  None.
*
* Return:
*  Current alarm value of the day of year.
*
* Global variables:
*  RTC_alarmCfgTimeDate.DayOfYear: the current day of year alarm
*  value is used.
*
*******************************************************************************/
uint16 RTC_ReadAlarmDayOfYear(void) 
{
    /* Return current alarm day of the year */
    return  (RTC_alarmCfgTimeDate.DayOfYear);
}


/*******************************************************************************
* Function Name:   RTC_WriteAlarmMask
********************************************************************************
*
* Summary:
*  Writes the Alarm Mask software register with 1 bit per time/date entry. Alarm
*  true when all masked time/date values match Alarm values.
*
* Parameters:
*  mask: Alarm Mask software register value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_alarmCfgMask: global variable which stores masks for
*  time/date alarm configuration is modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteAlarmMask(uint8 mask) 
{
    RTC_alarmCfgMask = mask;
}


/*******************************************************************************
* Function Name:   RTC_WriteIntervalMask
********************************************************************************
*
* Summary:
*  Writes the Interval Mask software register with 1 bit per time/date entry.
*  Interrupt true when any masked time/date overflow occur.
*
* Parameters:
*  mask: Alarm Mask software register value.
*
* Return:
*  None.
*
* Global variables:
*  RTC_intervalCfgMask: this global variable is modified - the new
*  value of interval mask is stored here.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RTC_WriteIntervalMask(uint8 mask) 
{
    RTC_intervalCfgMask = mask;
}


/*******************************************************************************
* Function Name:   RTC_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the Status software register which has flags for DST
*  (DST), Leap Year (LY) and AM/PM (AM_PM), Alarm active (AA).
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  RTC_statusDateTime: global variable is modified - active alarm
*  status bit is cleared.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Alarm active(AA) flag clear after read.
*
*******************************************************************************/
uint8 RTC_ReadStatus(void) 
{
    uint8 status;

    /* Save status */
    status = (uint8)RTC_statusDateTime;

    /* Clean AA flag after read of Status Register */
    RTC_statusDateTime &= (uint8)(~RTC_STATUS_AA);

    return (status);
}


/*******************************************************************************
* Function Name:   RTC_DayOfWeek
********************************************************************************
*
* Summary:
*  Calculates Day Of Week value use Zeller's congruence.
*
* Parameters:
*  dayOfMonth: Day Of Month value.
*  month: Month value.
*  year: Year value.
*
* Return:
*  Day Of Week value.
*
*******************************************************************************/
static uint8 RTC_DayOfWeek(uint8 dayOfMonth, uint8 month, uint16 year)

{
    /* Calculated sequence ((31 * month) / 12) mod 7 from the Zeller's congruence */
    static const uint8 CYCODE RTC_monthTemplate[RTC_MONTHS_IN_YEAR] =
                                                            {0u, 3u, 2u, 5u, 0u, 3u, 5u, 1u, 4u, 6u, 2u, 4u};

    /* It is simpler to handle the modified year, which is year - 1 during
    * January and February
    */
    if(month < RTC_MARCH)
    {
        year = year - 1u;
    }

    /* For Gregorian calendar: d = (day + y + y/4 - y/100 + y/400 + (31*m)/12) mod 7 */
    return ((uint8)(((year + (((year/4u) - (year/100u)) + (year/400u))) +
    (((uint16)RTC_monthTemplate[month - 1u]) + ((uint16) dayOfMonth))) % RTC_DAYS_IN_WEEK));
}


/*******************************************************************************
* Function Name:   RTC_SetInitValues
********************************************************************************
*
* Summary:
*    Does all initial calculation.
*    - Set LP Year flag;
*    - Set AM/PM flag;
*    - DayOfWeek;
*    - DayOfYear;
*    - Set DST flag;
*    - Convert relative to absolute date.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  RTC_currentTimeDate, RTC_dstTimeDateStart,
*  RTC_dstTimeDateStop, RTC_dstTimeDateStart,
*  RTC_alarmCfgTimeDate, RTC_statusDateTime,
*  RTC_dstStartStatus, RTC_dstStartStatus,
*  RTC_dstStopStatus, RTC_alarmCurStatus:
*  global variables are used while the initial calculation.
*
* RTC_dstTimeDateStart, RTC_currentTimeDate,
*  RTC_statusDateTime, RTC_dstStartStatus,
*  RTC_dstStartStatus, RTC_dstStopStatus,
*  RTC_alarmCurStatus: global variables are modified with the
*  initial calculated data.
*
* Reentrant:
*  No.
*
*******************************************************************************/
static void RTC_SetInitValues(void) 
{
    uint8 i;
    uint8 RTC_alarmCfgMaskTemp;

    /* Clears day of month counter */
    RTC_currentTimeDate.DayOfYear = 0u;

    /* Increments day of year value with day in current month */
    RTC_currentTimeDate.DayOfYear += RTC_currentTimeDate.DayOfMonth;

    /* Check leap year */
    if(1u == RTC_LEAP_YEAR(RTC_currentTimeDate.Year))
    {
        /* Set LP Year flag */
        RTC_statusDateTime |= RTC_STATUS_LY;
    }   /* leap year flag was set */
    else
    {
        /* Clear LP Year flag */
        RTC_statusDateTime &= (uint8)(~RTC_STATUS_LY);
    }   /* leap year flag was cleared */

    /* Day of Year */
    for(i = 0u; i < (RTC_currentTimeDate.Month - 1u); i++)
    {
        /* Increment on days in passed months */
        RTC_currentTimeDate.DayOfYear += RTC_daysInMonths[i];
    }   /* day of year is calculated */

    /* Leap year check */
    if(0u != RTC_IS_BIT_SET(RTC_statusDateTime, RTC_STATUS_LY))
    {
        /* Leap day check */
        if(RTC_currentTimeDate.Month > RTC_FEBRUARY)
        {
            /* Add leap day */
            RTC_currentTimeDate.DayOfYear++;
        }   /* Do nothing for non leap day */
    }   /* Do nothing for not leap year */

    /* DayOfWeek */
    RTC_currentTimeDate.DayOfWeek = RTC_DayOfWeek(
                                                                        RTC_currentTimeDate.DayOfMonth,
                                                                        RTC_currentTimeDate.Month,
                                                                        RTC_currentTimeDate.Year) + 1u;

    if(RTC_currentTimeDate.DayOfWeek > RTC_START_OF_WEEK)
    {
        RTC_currentTimeDate.DayOfWeek -= RTC_START_OF_WEEK;
    }
    else
    {
        RTC_currentTimeDate.DayOfWeek = RTC_DAYS_IN_WEEK -
                                        (RTC_START_OF_WEEK - RTC_currentTimeDate.DayOfWeek);
    }

    #if (1u == RTC_DST_FUNC_ENABLE)

        /* If DST values is given in a relative manner, converts to the absolute values */
        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_RELDATE))
        {
            RTC_DSTDateConversion();
        }

        /* Sets DST status respect to the DST start date and time */
        if(RTC_currentTimeDate.Month > RTC_dstTimeDateStart.Month)
        {
            RTC_statusDateTime |= RTC_STATUS_DST;
        }
        else if(RTC_currentTimeDate.Month == RTC_dstTimeDateStart.Month)
        {
            if(RTC_currentTimeDate.DayOfMonth > RTC_dstTimeDateStart.DayOfMonth)
            {
                RTC_statusDateTime |= RTC_STATUS_DST;
            }
            else if(RTC_currentTimeDate.DayOfMonth == RTC_dstTimeDateStart.DayOfMonth)
            {
                if(RTC_currentTimeDate.Hour > RTC_dstTimeDateStart.Hour)
                {
                    RTC_statusDateTime |= RTC_STATUS_DST;
                }
            }
            else
            {
                /* Do nothing if current day of month is less than DST stop day of month */
            }
        }
        else
        {
            /* Do nothing if current month is before than DST stop month */
        }

        /* Clears DST status respect to the DST start date and time */
        if(RTC_currentTimeDate.Month > RTC_dstTimeDateStop.Month)
        {
            RTC_statusDateTime &= (uint8)(~RTC_STATUS_DST);
        }
        else if(RTC_currentTimeDate.Month == RTC_dstTimeDateStop.Month)
        {
            if(RTC_currentTimeDate.DayOfMonth > RTC_dstTimeDateStop.DayOfMonth)
            {
                RTC_statusDateTime &= (uint8)(~RTC_STATUS_DST);
            }
            else if(RTC_currentTimeDate.DayOfMonth == RTC_dstTimeDateStop.DayOfMonth)
            {
                if(RTC_currentTimeDate.Hour > RTC_dstTimeDateStop.Hour)
                {
                    RTC_statusDateTime &= (uint8)(~RTC_STATUS_DST);
                }
            }
            else
            {
                /* Do nothing if current day of month is less than DST stop day of month */
            }
        }
        else
        {
            /* Do nothing if current month is before than DST stop month */
        }

        /* Clear DST start/stop statuses */
        RTC_dstStartStatus = 0u;
        RTC_dstStopStatus = 0u;

        /* Sets DST stop status month flag if DST stop month is equal to the
        * current month, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStop.Month == RTC_currentTimeDate.Month)
        {
            RTC_dstStopStatus |= RTC_DST_MONTH;
        }
        else
        {
            RTC_dstStopStatus &= (uint8)(~RTC_DST_MONTH);
        }

        /* Sets DST start status month flag if DST start month is equal to the
        * current month, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStart.Month == RTC_currentTimeDate.Month)
        {
            RTC_dstStartStatus |= RTC_DST_MONTH;
        }
        else
        {
            RTC_dstStartStatus &= (uint8)(~RTC_DST_MONTH);
        }

        /* Sets DST stop status day of month flag if DST stop day of month is
        * equal to the current day of month, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStop.DayOfMonth == RTC_currentTimeDate.DayOfMonth)
        {
            RTC_dstStopStatus |= RTC_DST_DAYOFMONTH;
        }
        else
        {
            RTC_dstStopStatus &= (uint8)(~RTC_DST_DAYOFMONTH);
        }

        /* Sets DST start status day of month flag if DST start day of month is
        * equal to the current day of month, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStart.DayOfMonth == RTC_currentTimeDate.DayOfMonth)
        {
            RTC_dstStartStatus |= RTC_DST_DAYOFMONTH;
        }
        else
        {
            RTC_dstStartStatus &= (uint8)(~RTC_DST_DAYOFMONTH);
        }

        /* Sets DST stop status hour flag if DST stop hour is equal to the
        * current hour, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStop.Hour == RTC_currentTimeDate.Hour)
        {
            RTC_dstStopStatus |= RTC_DST_HOUR;
        }
        else
        {
            RTC_dstStopStatus &= (uint8)(~RTC_DST_HOUR);
        }

        /* Sets DST start status hour flag if DST start hour is equal to the
        * current hour, otherwise clears that flag.
        */
        if(RTC_dstTimeDateStart.Hour == RTC_currentTimeDate.Hour)
        {
            RTC_dstStartStatus |= RTC_DST_HOUR;
        }
        else
        {
            RTC_dstStartStatus &= (uint8)(~RTC_DST_HOUR);
        }

        /* DST Enable ? */
        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_ENABLE))
        {
            if(0u != RTC_IS_BIT_SET(RTC_statusDateTime, RTC_STATUS_DST))
            {
                if(0u != RTC_IS_BIT_SET(RTC_dstStartStatus,
                        (RTC_DST_HOUR | RTC_DST_DAYOFMONTH | RTC_DST_MONTH)))
                {
                    /* Subtract current minutes value with minutes value, what
                    *  are out of full hour in DST offset.
                    */
                    RTC_currentTimeDate.Min -= RTC_dstOffsetMin %
                                                            (RTC_HOUR_ELAPSED + 1u);

                    /* If current minutes value is greater than number of
                    * minutes in hour - could be only if hour's value is negative
                    */
                    if(RTC_currentTimeDate.Min > RTC_HOUR_ELAPSED)
                    {
                        /* Adjust current minutes value. Convert to the positive. */
                        RTC_currentTimeDate.Min = RTC_HOUR_ELAPSED -
                                                               ((uint8)(~RTC_currentTimeDate.Min));

                        /* Decrement current hours value. */
                        RTC_currentTimeDate.Hour--;
                    }

                    /* Subtract current hours value with hours value, what
                    *  are full part of hours in DST offset.
                    */
                    RTC_currentTimeDate.Hour -= RTC_dstOffsetMin /
                                                             (RTC_HOUR_ELAPSED + 1u);

                    /* Check if current hour's value is negative */
                    if(RTC_currentTimeDate.Hour > RTC_DAY_ELAPSED)
                    {
                        /* Adjust hour */
                        RTC_currentTimeDate.Hour = RTC_DAY_ELAPSED -
                                                                ((uint8)(~RTC_currentTimeDate.Hour));

                        /* Decrement day of month, year and week */
                        RTC_currentTimeDate.DayOfMonth--;
                        RTC_currentTimeDate.DayOfYear--;
                        RTC_currentTimeDate.DayOfWeek--;

                        if(0u == RTC_currentTimeDate.DayOfWeek)
                        {
                            RTC_currentTimeDate.DayOfWeek = RTC_DAYS_IN_WEEK;
                        }

                        if(0u == RTC_currentTimeDate.DayOfMonth)
                        {
                            /* Decrement months value */
                            RTC_currentTimeDate.Month--;

                            /* The current month is month before 1st one. */
                            if(0u == RTC_currentTimeDate.Month)
                            {
                                /* December is the month before January */
                                RTC_currentTimeDate.Month = RTC_DECEMBER;
                                RTC_currentTimeDate.DayOfMonth =
                                            RTC_daysInMonths[RTC_currentTimeDate.Month - 1u];

                                /* Decrement years value */
                                RTC_currentTimeDate.Year--;
                                if(1u == RTC_LEAP_YEAR(RTC_currentTimeDate.Year))
                                {
                                    /* Set leap year status flag */
                                    RTC_statusDateTime |= RTC_STATUS_LY;
                                    RTC_currentTimeDate.DayOfYear = RTC_DAYS_IN_LEAP_YEAR;
                                }
                                else
                                {
                                    /* Clear leap year status flag */
                                    RTC_statusDateTime &= (uint8)(~RTC_STATUS_LY);
                                    RTC_currentTimeDate.DayOfYear = RTC_DAYS_IN_YEAR;
                                }
                            }   /* 0u == RTC_currentTimeDate.Month */
                            else
                            {
                                RTC_currentTimeDate.DayOfMonth =
                                            RTC_daysInMonths[RTC_currentTimeDate.Month - 1u];
                            }   /* 0u != End of RTC_currentTimeDate.Month */
                        }   /* 0u == End of RTC_currentTimeDate.DayOfMonth */
                    }   /* End of RTC_currentTimeDate.Hour > RTC_DAY_ELAPSED */

                    /* Clear DST status flag */
                    RTC_statusDateTime &= (uint8)(~RTC_STATUS_DST);
                    /* Clear DST stop status */
                    RTC_dstStopStatus = 0u;
                }
            }
            else    /* Current time and date DO NOT match DST time and date */
            {
                if(0u != RTC_IS_BIT_SET(RTC_dstStartStatus, RTC_DST_HOUR |
                                                     RTC_DST_DAYOFMONTH | RTC_DST_MONTH))
                {
                    /* Add Hour and Min */
                    RTC_currentTimeDate.Min +=
                                                RTC_dstOffsetMin % (RTC_HOUR_ELAPSED + 1u);

                    if(RTC_currentTimeDate.Min > RTC_HOUR_ELAPSED)
                    {
                        /* Adjust Min */
                        RTC_currentTimeDate.Min -= (RTC_HOUR_ELAPSED + 1u);
                        RTC_currentTimeDate.Hour++;
                    }

                    RTC_currentTimeDate.Hour +=
                                                RTC_dstOffsetMin / (RTC_HOUR_ELAPSED + 1u);
                    if(RTC_currentTimeDate.Hour > RTC_DAY_ELAPSED)
                    {
                        /* Adjust hour, add day */
                        RTC_currentTimeDate.Hour -= (RTC_DAY_ELAPSED + 1u);
                        RTC_currentTimeDate.DayOfMonth++;
                        RTC_currentTimeDate.DayOfYear++;
                        RTC_currentTimeDate.DayOfWeek++;

                        if(RTC_currentTimeDate.DayOfWeek > RTC_WEEK_ELAPSED)
                        {
                            RTC_currentTimeDate.DayOfWeek = 1u;
                        }

                        if(RTC_currentTimeDate.DayOfMonth >
                                            RTC_daysInMonths[RTC_currentTimeDate.Month - 1u])
                        {
                            RTC_currentTimeDate.Month++;
                            RTC_currentTimeDate.DayOfMonth = 1u;

                            /* Has new year come? */
                            if(RTC_currentTimeDate.Month > RTC_YEAR_ELAPSED)
                            {
                                /* Set first month of the year */
                                RTC_currentTimeDate.Month = RTC_JANUARY;

                                /* Increment year */
                                RTC_currentTimeDate.Year++;

                                /* Update leap year status */
                                if(1u == RTC_LEAP_YEAR(RTC_currentTimeDate.Year))
                                {
                                    /* LP - true, else - false */
                                    RTC_statusDateTime |= RTC_STATUS_LY;
                                }
                                else
                                {
                                    RTC_statusDateTime &= (uint8)(~RTC_STATUS_LY);
                                }

                                /* Set day of year to the first one */
                                RTC_currentTimeDate.DayOfYear = 1u;
                            }
                        }
                    }
                    RTC_statusDateTime |= RTC_STATUS_DST;
                    RTC_dstStartStatus = 0u;
                }
            }
        }
    #endif /* 1u == RTC_DST_FUNC_ENABLE */

    /* Set AM/PM flag */
    if(RTC_currentTimeDate.Hour < RTC_HALF_OF_DAY_ELAPSED)
    {
        /* AM Hour 00:00-11:59, flag zero */
        RTC_statusDateTime &= (uint8)(~RTC_STATUS_AM_PM);
    }
    else
    {
        /* PM Hour 12:00 - 23:59, flag set */
        RTC_statusDateTime |= RTC_STATUS_AM_PM;
    }

    /* Alarm calculation */

    /* Alarm SEC */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_SEC_MASK))
    {
        if(RTC_alarmCfgTimeDate.Sec == RTC_currentTimeDate.Sec)
        {
            /* Set second alarm */
            RTC_alarmCurStatus |= RTC_ALARM_SEC_MASK;
        }
        else
        {
            /* Clear second alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_SEC_MASK);
        }
    }

    /* Alarm MIN */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_MIN_MASK))
    {
        if(RTC_alarmCfgTimeDate.Min == RTC_currentTimeDate.Min)
        {
            /* Set minute alarm */
            RTC_alarmCurStatus |= RTC_ALARM_MIN_MASK;
        }
        else
        {
            /* Clear minute alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_MIN_MASK);
        }
    }

    /* Alarm HOUR */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_HOUR_MASK))
    {
        if(RTC_alarmCfgTimeDate.Hour == RTC_currentTimeDate.Hour)
        {
            /* Set hour alarm */
            RTC_alarmCurStatus |= RTC_ALARM_HOUR_MASK;
        }
        else
        {
            /* Clear hour alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_HOUR_MASK);
        }
    }

    /* Alarm DAYOFWEEK */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_DAYOFWEEK_MASK))
    {
        if(RTC_alarmCfgTimeDate.DayOfWeek == RTC_currentTimeDate.DayOfWeek)
        {
            /* Set day of week alarm */
            RTC_alarmCurStatus |= RTC_ALARM_DAYOFWEEK_MASK;
        }
        else
        {
            /* Clear day of week alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFWEEK_MASK);
        }
    }

    /* Alarm DAYOFYEAR */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_DAYOFYEAR_MASK))
    {
        if(RTC_alarmCfgTimeDate.DayOfYear == RTC_currentTimeDate.DayOfYear)
        {
            /* Set day of year alarm */
            RTC_alarmCurStatus |= RTC_ALARM_DAYOFYEAR_MASK;
        }
        else
        {
            /* Clear day of year alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFYEAR_MASK);
        }
    }

    /* Alarm DAYOFMONTH */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_DAYOFMONTH_MASK))
    {
        if(RTC_alarmCfgTimeDate.DayOfMonth == RTC_currentTimeDate.DayOfMonth)
        {
            /* Set day of month alarm */
            RTC_alarmCurStatus |= RTC_ALARM_DAYOFMONTH_MASK;
        }
           else
        {
            /* Clear day of month alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_DAYOFMONTH_MASK);
        }
    }

    /* Alarm MONTH */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_MONTH_MASK))
    {
        if(RTC_alarmCfgTimeDate.Month == RTC_currentTimeDate.Month)
        {
            /* Set month alarm */
            RTC_alarmCurStatus |= RTC_ALARM_MONTH_MASK;
        }
        else
        {
            /* Clear month alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_MONTH_MASK);
        }
    }

    /* Alarm YEAR */
    if(0u != RTC_IS_BIT_SET(RTC_alarmCfgMask, RTC_ALARM_YEAR_MASK))
    {
        if(RTC_alarmCfgTimeDate.Year == RTC_currentTimeDate.Year)
        {
            /* Set year alarm */
            RTC_alarmCurStatus |= RTC_ALARM_YEAR_MASK;
        }
        else
        {
            /* Clear year alarm */
            RTC_alarmCurStatus &= (uint8)(~RTC_ALARM_YEAR_MASK);
        }
    }

    RTC_alarmCfgMaskTemp = RTC_alarmCfgMask;
    /* Set Alarm flag event */
    RTC_SET_ALARM(RTC_alarmCfgMaskTemp,
                               RTC_alarmCurStatus,
                               RTC_statusDateTime);
}


#if (1u == RTC_DST_FUNC_ENABLE)
    /*******************************************************************************
    * Function Name:   RTC_WriteDSTMode
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST mode software register. That enables or disables DST changes
    *  and sets the date mode to fixed date or relative date. Only generated if DST
    *  enabled.
    *
    * Parameters:
    *  mode: DST Mode software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstModeType: global variable is modified with the new
    *  DST mode type: relative or fixed.
    *
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week:
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Week,
    *  RTC_currentTimeDate.Year: for the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_dstTimeDateStart.DayOfMonth,
    *  RTC_dstTimeDateStop.DayOfMonth: updated after convertion by
    *  the RTC_DSTDateConversion() function if DST mode is
    *  configured to be relative.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTMode(uint8 mode) 
    {
        /* Set DST mode */
        RTC_dstModeType = mode;

        if(0u != RTC_IS_BIT_SET(mode, RTC_DST_RELDATE))
        {
            /* Convert DST date */
            RTC_DSTDateConversion();
        }
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStartHour
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Start Hour software register. Used for absolute date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  hour: DST Start Hour software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.Hour: global variable is modified with
    *  the new value.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStartHour(uint8 hour) 
    {
        /* Set DST Start Hour */
        RTC_dstTimeDateStart.Hour = hour;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStartOfMonth
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Start DayOfMonth software register. Used for absolute date
    *  entry. Only generated if DST is enabled.
    *
    * Parameters:
    *  dayOfMonth: DST Start DayOfMonth software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.DayOfMonth: global variable is modified
    *  with the new value.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStartDayOfMonth(uint8 dayOfMonth)
         
    {
        /* Set DST Start day of month */
        RTC_dstTimeDateStart.DayOfMonth = dayOfMonth;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStartMonth
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Start Month software register. Used for absolute date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  month: DST Start month software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.Month: global variable is modified
    *  with the new value.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStartMonth(uint8 month) 
    {
        /* Set DST Start month */
        RTC_dstTimeDateStart.Month = month;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStartDayOfWeek
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Start DayOfWeek software register. Used for absolute date
    *  entry. Only generated if DST is enabled.
    *
    * Parameters:
    *  dayOfWeek: DST start day of week software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstModeType: global variable, where DST mode type:
    *  relative or fixed is stored.
    *
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week,
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Week: for the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_currentTimeDate.Year: for the day of week calculation,
    *   it is used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_dstTimeDateStart.DayOfWeek: global variable is modified
    *  with the new day of week value.
    *
    *  RTC_dstTimeDateStart.DayOfMonth and
    *  RTC_dstTimeDateStop.DayOfMonth are modified by
    *  the RTC_DSTDateConversion() function if DST mode is
    *  configured to be relative.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStartDayOfWeek(uint8 dayOfWeek)
         
    {
        /* Set DST Start day of week */
        RTC_dstTimeDateStart.DayOfWeek = dayOfWeek;

        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_RELDATE))
        {
            /* Convert DST date */
            RTC_DSTDateConversion();
        }
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStartWeek
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Start Week software register. Used for absolute date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  week: DST start week software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.Week: global variable is modified with
    *   the new week's value of the DST start time/date.
    *
    *  RTC_dstTimeDateStart.DayOfMonth,
    *  RTC_dstTimeDateStop.DayOfMonth: is modified after convertion
    *  by the RTC_DSTDateConversion() function if DST mode is
    *  configured to be relative.
    *
    *  RTC_dstModeType: global variable is used for theDST mode
    *   type: relative or fixed store.
    *
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week: for the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Week: for the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_currentTimeDate.Year: for the day of week calculation,
    *   it is used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStartWeek(uint8 week) 
    {
        /* Set DST Start week */
        RTC_dstTimeDateStart.Week = week;

        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_RELDATE))
        {
            /* Convert DST date */
            RTC_DSTDateConversion();
        }
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStopHour
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Stop Hour software register. Used for absolute date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  hour: DST stop hour software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStart.Hour: global variable is modified with the new hour
    *   of the DST start time/date.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStopHour(uint8 hour) 
    {
        /* Set DST Stop hour */
        RTC_dstTimeDateStop.Hour = hour;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStopDayOfMonth
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Stop DayOfMonth software register. Used for absolute date
    *  entry. Only generated if DST is enabled.
    *
    * Parameters:
    *  dayOfMonth: DST stop day of month software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStop.DayOfMonth: global variable is modified
    *  where new day of month's value of the DST stop time/date.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStopDayOfMonth(uint8 dayOfMonth)
         
    {
        /* Set DST Start day of month */
        RTC_dstTimeDateStop.DayOfMonth = dayOfMonth;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStopMonth
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Stop Month software  register. Used for absolute date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  month: DST Stop Month software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStop.Month: global variable is modified with
    *   the new month of the DST stop time/date.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStopMonth(uint8 month) 
    {
        /* Set DST Stop month */
        RTC_dstTimeDateStop.Month = month;
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStopDayOfWeek
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Stop DayOfWeek software register. Used for relative date
    *  entry. Only generated if DST is enabled.
    *
    * Parameters:
    *  dayOfWeek: DST stop day of week software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStop.DayOfWeek: global variable is modified
    *   with the day of week of the DST stop time/date.
    *
    *  RTC_dstModeType: global variable is used to store DST mode
    *   type: relative or fixed.
    *
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week,
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Weekfor the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_currentTimeDate.Year: for the day of week calculation,
    *   it is used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_dstTimeDateStop.DayOfWeek: global variable is modified
    *  with the new day of week's value.
    *
    *  RTC_dstTimeDateStart.DayOfMonth and
    *  RTC_dstTimeDateStop.DayOfMonth are modified by
    *  the RTC_DSTDateConversion() function if DST mode is
    *  configured to be relative.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStopDayOfWeek(uint8 dayOfWeek)
         
    {
        /* Set DST Stop day of week */
        RTC_dstTimeDateStop.DayOfWeek = dayOfWeek;

        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_RELDATE))
        {
            /* Convert DST date */
            RTC_DSTDateConversion();
        }
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTStopWeek
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Stop Week software register. Used for relative date entry.
    *  Only generated if DST is enabled.
    *
    * Parameters:
    *  week: DST stop week software register value.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstTimeDateStop.Week: global variable used to store the
    *  DST stop time/date is stored.
    *
    *  RTC_dstModeType: global variable is used to store DST mode
    *   type: relative or fixed.
    *
    *  RTC_dstTimeDateStart.Month,
    *  RTC_dstTimeDateStart.DayOfWeek,
    *  RTC_dstTimeDateStart.Week,
    *  RTC_dstTimeDateStop.Month,
    *  RTC_dstTimeDateStop.DayOfWeek,
    *  RTC_dstTimeDateStop.Week: used for the day of week correction,
    *   they are used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_currentTimeDate.Year: for the day of week calculation,
    *   it is used by RTC_DSTDateConversion() function if DST
    *   mode is configured to be relative.
    *
    *  RTC_dstTimeDateStop.Week: global variable is modified with
    *  the new value.
    *
    *  RTC_dstTimeDateStart.DayOfMonth and
    *  RTC_dstTimeDateStop.DayOfMonth are modified by
    *  the RTC_DSTDateConversion() function if DST mode is
    *  configured to be relative.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTStopWeek(uint8 week) 
    {
        /* Set DST Stop week */
        RTC_dstTimeDateStop.Week = week;

        if(0u != RTC_IS_BIT_SET(RTC_dstModeType, RTC_DST_RELDATE))
        {
            /* Convert DST date */
            RTC_DSTDateConversion();
        }
    }


    /*******************************************************************************
    * Function Name:   RTC_WriteDSTOffset
    ********************************************************************************
    *
    * Summary:
    *  Writes the DST Offset register. Allows a configurable increment or decrement
    *  of time between 0 and 255 minutes. Increment occurs on DST Start and
    *  decrement on DST Stop. Only generated if DST is enabled.
    *
    * Parameters:
    *  offset: The DST offset to be saved.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  RTC_dstOffsetMin: global variable is modified with the new
    *  DST offset value (in minutes).
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RTC_WriteDSTOffset(uint8 offset) 
    {
        /* Set DST offset */
        RTC_dstOffsetMin = offset;
    }

#endif /* 1u == RTC_DST_FUNC_ENABLE */


/* [] END OF FILE */
