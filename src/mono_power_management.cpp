// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_power_management.h"
#include "application_context_interface.h"
//#include "consoles.h"
#include <project.h>


#ifdef DEVICE_SERIAL
#include <serial_usb_api.h>
extern char serial_usbuart_is_powered;
#endif

using namespace mono::power;

bool MonoPowerManagement::__RTCFired = false;

MonoPowerManagement::MonoPowerManagement()
{
    batteryLowFlag = batteryEmptyFlag = false;
    powerAwarenessQueue = NULL;
    PowerSystem = &powerSubsystem;

    PowerSystem->BatteryLowHandler.attach<MonoPowerManagement>(this, &MonoPowerManagement::systemLowBattery);
    PowerSystem->BatteryEmptyHandler.attach<MonoPowerManagement>(this, &MonoPowerManagement::systemEmptyBattery);
    singleShot = false;
    IApplicationContext::Instance->RunLoop->addDynamicTask(this);

#ifdef DEVICE_SERIAL
    // if mbed device serial exists, enable if usb powered
    serial_usbuart_is_powered = PowerSystem->IsUSBCharging();
#endif

    //turn on peripherals
    PowerSystem->setPowerFence(false);
}

void MonoPowerManagement::EnterSleep(bool skipAwarenessQueues)
{
    if (!skipAwarenessQueues)
        processSleepAwarenessQueue();

    PowerSystem->onSystemEnterSleep();

    powerSubsystem.setPowerFence(true);
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
    PowerSystem->onSystemWakeFromSleep();

    powerSubsystem.setPowerFence(false);

    //mono::defaultSerial.printf("Wake up! Restore clocks and read status regs: 0x%x\r\n", status);

    if (!skipAwarenessQueues)
        processWakeAwarenessQueue();

}


void MonoPowerManagement::taskHandler()
{
    if (batteryLowFlag)
    {
        batteryLowFlag = false;
        IPowerManagement::processBatteryLowAwarenessQueue();
    }

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
    CyPins_SetPinDriveMode(USER_SW, CY_PINS_DM_RES_UP);

    // EXP_PWR_EN must be strong in sleep!
    CyPins_SetPinDriveMode(VAUX_EN, CY_PINS_DM_STRONG);
    // EXP_3V3_EN toggle must also be strong drive
    CyPins_SetPinDriveMode(VAUX_EN, CY_PINS_DM_STRONG);

    //Power INT res pull up in sleep
    CyPins_SetPinDriveMode(CYREG_PRT5_PC2, CY_PINS_DM_RES_UP);

    CyPins_SetPinDriveMode(A5, CY_PINS_DM_RES_DWN);
    CyPins_ClearPin(A5);

}

void MonoPowerManagement::powerDownMCUPeripherals()
{
    PWM_Sleep();
//    SPI_SD_Sleep();
    SPI1_Sleep();
//    SPI_RP_Sleep();
    ADC_SAR_1_Sleep();
//    I2C_Sleep();

    I2C_Sleep();

#ifdef DEVICE_SERIAL
    if (serial_usbuart_is_started())
    {
//        USBUART_SaveConfig();
//        USBUART_Suspend();
        USBUART_Stop();
        serial_usbuart_stopped();
    }
#endif

    saveDMRegisters();
    setupMCUPeripherals();
}

void MonoPowerManagement::powerUpMCUPeripherals()
{
    restoreDMRegisters();

#ifdef DEVICE_SERIAL
    serial_usbuart_stopped();
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

    //mbed::Serial::wakeUpRoutine();
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

/// MARK: Power Sub-System Battery callbacks

void MonoPowerManagement::systemLowBattery()
{
    batteryLowFlag = true;
}

void MonoPowerManagement::systemEmptyBattery()
{
    //batteryEmptyFlag = true;

    while (!PowerSystem->IsPowerOk())
    {
        debug("Sleep until power is OK!\t\n");
        EnterSleep();
    }

    IApplicationContext::SoftwareResetToApplication();
}
