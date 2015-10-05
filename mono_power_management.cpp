//
//  mono_power_management.cpp
//  mono
//
//  Created by Kristoffer Lyder Andersen on 09/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "mono_power_management.h"
#include "consoles.h"

using namespace mono::power;

MonoPowerManagement::MonoPowerManagement()
{
    powerAwarenessQueue = NULL;
    
    
    //
    
}

void MonoPowerManagement::EnterSleep()
{
    processSleepAwarenessQueue();
    
    mono::defaultSerial.printf("Going to sleep...\n\r");
    //PowerSystem->onSystemEnterSleep();
    wait_ms(10);
    
    powerDownMCUPeripherals();
    
    
    //CyILO_Start1K(); // make sure the 1K ILO Osc is running
    //CyMasterClk_SetSource(CY_MASTER_SOURCE_IMO);
    CyPmSaveClocks();
    
    //CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_PICU);
    CyPmHibernate();
    
    CyPmRestoreClocks();
    
    int status = CyPmReadStatus(CY_PM_FTW_INT | CY_PM_CTW_INT | CY_PM_ONEPPS_INT);
    
    //CyMasterClk_SetSource(CY_MASTER_SOURCE_PLL);
    //CyGlobalIntEnable;
    powerUpMCUPeripherals();
    //PowerSystem->onSystemWakeFromSleep();
    
    mono::defaultSerial.printf("Wake up! Restore clocks and read status regs: 0x%x\n\r", status);
    
    
    processWakeAwarenessQueue();
}



void MonoPowerManagement::processResetAwarenessQueue()
{
    setupMCUPeripherals();
    CyPins_SetPinDriveMode(CYREG_PRT4_PC7, CY_PINS_DM_RES_UPDWN);
    CyPins_SetPinDriveMode(TFT_LED_PWR, CY_PINS_DM_RES_DWN);
    CyPins_ClearPin(TFT_LED_PWR);
    
    IPowerManagement::processResetAwarenessQueue();
}


void MonoPowerManagement::setupMCUPeripherals()
{
    // set all PORT 0 (Exp. conn. to res pull down)
    CY_SET_REG8(CYREG_PRT0_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT0_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT0_DM2, 0x00);
    
    // set all PORT 1 (Exp. conn. to res pull down)
    CY_SET_REG8(CYREG_PRT1_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT1_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT1_DM2, 0x00);
    
    // set all PORT 2 (RP spi to res pull down)
    CY_SET_REG8(CYREG_PRT2_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT2_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT2_DM2, 0x00);
    
    // set all PORT 3 (TFT to res pull down)
    CY_SET_REG8(CYREG_PRT3_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT3_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT3_DM2, 0x00);
    
    // set all PORT 4 (exp. conn to res pull down)
    CY_SET_REG8(CYREG_PRT4_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT4_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT4_DM2, 0x00);
    
    // set all PORT 5 (switches and inputs, to res pull down)
    CY_SET_REG8(CYREG_PRT5_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT5_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT5_DM2, 0x00);
    
    // set all PORT 6 (TFT conn. to res pull down)
    CY_SET_REG8(CYREG_PRT6_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT6_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT6_DM2, 0x00);
    
    // set all PORT 12 (misc. to res pull down)
    CY_SET_REG8(CYREG_PRT12_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT12_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT12_DM2, 0x00);
    
    //CY_SET_REG8(CYREG_PRT12_SIO_CFG, 0x00); // SIO non-reg output
    
    CY_SET_REG8(CYREG_PRT15_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT15_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT15_DM2, 0x00);
    
}

void MonoPowerManagement::powerDownMCUPeripherals()
{
    PWM_Sleep();
    SPI0_Sleep();
    SPI1_Sleep();
    I2C_Sleep();
#ifndef MONO_NO_USB
    USBUART_Stop();
#endif
    CyPins_ClearPin(CYREG_PRT4_PC7);
}

void MonoPowerManagement::powerUpMCUPeripherals()
{
    CyPins_SetPin(CYREG_PRT4_PC7);
//#ifndef MONO_NO_USB
//    USBUART_Start(0, USBUART_DWR_VDDD_OPERATION);
//#endif
    PWM_Wakeup();
    I2C_Wakeup();
    SPI0_Wakeup();
    SPI1_Wakeup();
    
    mbed::Serial::wakeUpRoutine();
}