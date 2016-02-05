//
//  mono_power_management.cpp
//  mono
//
//  Created by Kristoffer Lyder Andersen on 09/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "mono_power_management.h"
#include "consoles.h"
#include <project.h>

using namespace mono::power;

bool MonoPowerManagement::__RTCFired = false;

MonoPowerManagement::MonoPowerManagement()
{
    powerAwarenessQueue = NULL;
    PowerSystem = &powerSubsystem;
}

void MonoPowerManagement::EnterSleep()
{
    processSleepAwarenessQueue();
    
    mono::defaultSerial.printf("Going to sleep...\n\r");
    //PowerSystem->onSystemEnterSleep();
    wait_ms(10);
    
    powerSubsystem.setPowerFencePeripherals(true);
    powerSubsystem.powerOffUnused();
    powerDownMCUPeripherals();
    
    //CyILO_Start1K(); // make sure the 1K ILO Osc is running
    //CyMasterClk_SetSource(CY_MASTER_SOURCE_IMO);

    bool wakeup = false;
    while(wakeup == false)
    {
        wakeup = true; //wake up by defualt
        CyPmSaveClocks();

        __RTCFired = false;
        CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_PICU | PM_SLEEP_SRC_CTW);
        //CyPmHibernate();

        CyPmRestoreClocks();

        //int status = CyPmReadStatus(CY_PM_CTW_INT);
        if (__RTCFired)
        {
            wakeup = false;
            __RTCFired = false;
        }

    }
    
    //CyMasterClk_SetSource(CY_MASTER_SOURCE_PLL);
    //CyGlobalIntEnable;
    
    powerUpMCUPeripherals();
    //PowerSystem->onSystemWakeFromSleep();

    powerSubsystem.setPowerFencePeripherals(false);
    
    //mono::defaultSerial.printf("Wake up! Restore clocks and read status regs: 0x%x\n\r", status);
    
    processWakeAwarenessQueue();


}



void MonoPowerManagement::processResetAwarenessQueue()
{
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
    // set PRT2_2 as OD, drives low
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
    // set PRT5_0 as OD drives low
    CY_SET_REG8(CYREG_PRT5_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT5_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT5_DM2, 0x00);
    
    // set all PORT 6 (TFT conn. to res pull down)
    CY_SET_REG8(CYREG_PRT6_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT6_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT6_DM2, 0x00);
    
    // set all PORT 12 (misc. to res pull down)
    //set PRT12_5 as OD, drives low
    CY_SET_REG8(CYREG_PRT12_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT12_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT12_DM2, 0x00);
    
    //CY_SET_REG8(CYREG_PRT12_SIO_CFG, 0x00); // SIO non-reg output
    
    CY_SET_REG8(CYREG_PRT15_DM0, 0x00);
    CY_SET_REG8(CYREG_PRT15_DM1, 0x00);
    CY_SET_REG8(CYREG_PRT15_DM2, 0x00);
    
    // SW USER must be weak pull up in sleep!
    CyPins_SetPinDriveMode(SW_USER, CY_PINS_DM_RES_UP);
    
//    CyPins_SetPinDriveMode(RP_nRESET, CY_PINS_DM_OD_LO);
//    CyPins_ClearPin(RP_nRESET);
//    
//    CyPins_SetPinDriveMode(CYREG_PRT2_PC2, CY_PINS_DM_OD_LO);
//    CyPins_ClearPin(CYREG_PRT2_PC2);
//
//    // PC12_4 er sda i demo - ikke i PCBv3, her er det PRT12_5
//    CyPins_SetPinDriveMode(CYREG_PRT12_PC4, CY_PINS_DM_OD_LO);
//    CyPins_ClearPin(CYREG_PRT12_PC4);
    
    CyPins_SetPinDriveMode(ARD_A5, CY_PINS_DM_RES_DWN);
    CyPins_ClearPin(ARD_D5);
    
}

void MonoPowerManagement::powerDownMCUPeripherals()
{
    PWM_Sleep();
//    SPI_SD_Sleep();
    SPI1_Sleep();
//    SPI_RP_Sleep();
    ADC_SAR_1_Sleep();
//    I2C_Sleep();
    
#ifndef MONO_DISP_CTRL_HX8340
    SPI1_Sleep();
#endif
    
    I2C_Sleep();
#ifndef MONO_NO_USB
    USBUART_SaveConfig();
    USBUART_Suspend();
    //USBUART_Stop();
#endif
    
    saveDMRegisters();
    setupMCUPeripherals();
}

void MonoPowerManagement::powerUpMCUPeripherals()
{
    restoreDMRegisters();
    
#ifndef MONO_NO_USB
    USBUART_Resume();
    USBUART_RestoreConfig();
    //USBUART_Start(0, USBUART_DWR_VDDD_OPERATION);
#endif
    
    PWM_Wakeup();
    I2C_Wakeup();
    SPI1_Wakeup();
//    SPI_SD_Wakeup();
//    SPI_RP_Wakeup();
    ADC_SAR_1_Wakeup();
//    I2C_Wakeup();
    
#ifndef MONO_DISP_CTRL_HX8340
    SPI1_Wakeup();
#endif
    
    mbed::Serial::wakeUpRoutine();
}

void MonoPowerManagement::saveDMRegisters()
{
    uint8_t cnt = 0;
    
    saveDMPort(CYREG_PRT0_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT1_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT2_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT3_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT4_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT5_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT6_DM0,  DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT12_DM0, DriveModeRegisters+(cnt++)*3);
    saveDMPort(CYREG_PRT15_DM0, DriveModeRegisters+(cnt++)*3);
}

void MonoPowerManagement::restoreDMRegisters()
{
    uint8_t cnt = 0;
    
    restoreDMPort(CYREG_PRT0_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT1_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT2_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT3_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT4_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT5_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT6_DM0,  DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT12_DM0, DriveModeRegisters+(cnt++)*3);
    restoreDMPort(CYREG_PRT15_DM0, DriveModeRegisters+(cnt++)*3);
}

void MonoPowerManagement::saveDMPort(uint32_t regAddrOffset, uint8_t destOffset[])
{
    destOffset[0] = CY_GET_REG8(regAddrOffset);
    destOffset[1] = CY_GET_REG8(regAddrOffset+1);
    destOffset[2] = CY_GET_REG8(regAddrOffset+2);
}

void MonoPowerManagement::restoreDMPort(uint32_t regAddrOffset, uint8_t srcOffset[])
{
    CY_SET_REG8(regAddrOffset,   srcOffset[0]);
    CY_SET_REG8(regAddrOffset+1, srcOffset[1]);
    CY_SET_REG8(regAddrOffset+2, srcOffset[2]);
}