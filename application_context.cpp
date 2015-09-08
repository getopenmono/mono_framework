//
//  application_context.cpp
//  usbtest
//
//  Created by Kristoffer Andersen on 13/07/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "application_context.h"
#include "application_controller_interface.h"
#include <display_painter.h>
#include "consoles.h"

extern "C" {
#include <project.h>
}

using namespace mono;

ApplicationContext ApplicationContext::singleton;
IApplicationContext *IApplicationContext::Instance = NULL;

ApplicationContext::ApplicationContext() : dispController()
{
    PWM_Start();
    PWM_WriteCompare2(0);

    IApplicationContext::Instance = this;
    
    displayController = &dispController;
    PowerSystem = &pwrSystem;
    RunLoop = &runLoop;
    
}

int ApplicationContext::exec()
{
    runLoop.exec();
    
    return 0;
}

void ApplicationContext::setMonoApplication(mono::IApplication *monoApp)
{
    this->application = monoApp;
    mono::IApplicationContext::Instance->displayController->init();

    PowerSystem->onSystemPowerOnReset();
    
    
    
    monoApp->monoWakeFromReset();
}

void ApplicationContext::sleepForMs(uint32_t ms)
{
    uint8_t ticks = 1;
    while ((1u << ticks) < ms) {
        ticks++;
    }
    
    defaultSerial.printf("Setting CTW to: %i for ms: %i\n\r",ticks, ms);
    *((reg8*)CYREG_PM_TW_CFG1) = 0x0F & ticks;
    PM_TW_CFG2 = PM_TW_CFG2 | 0x0C; // CTW enable and CTW interrupts
    defaultSerial.printf("CTW CFG Reg: 0x%x\n\r",PM_TW_CFG2);
    enterSleepMode();
}

void ApplicationContext::enterSleepMode()
{
    this->application->monoWillGotoSleep();
    
    
    PWM_WriteCompare1(0);
    
    CyILO_Start1K(); // make sure the 1K ILO Osc is running
    
    mono::defaultSerial.printf("Going to sleep...\n\r");
    PowerSystem->onSystemEnterSleep();
    CyMasterClk_SetSource(CY_MASTER_SOURCE_IMO);
    CyPmSaveClocks();
    
    //CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_PICU);
    CyPmHibernate();
    
    int status = CyPmReadStatus(CY_PM_FTW_INT | CY_PM_CTW_INT | CY_PM_ONEPPS_INT);
    CyPmRestoreClocks();
    CyMasterClk_SetSource(CY_MASTER_SOURCE_PLL);
    
    PowerSystem->onSystemWakeFromSleep();
    
    mono::defaultSerial.printf("Wake up! Restore clocks and read status regs: 0x%x\n\r", status);
    
    PWM_WriteCompare1(128);
    
    
    this->application->monoWakeFromSleep();
    
}