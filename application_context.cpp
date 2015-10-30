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
#include <mbed_interface.h>

extern "C" {
#include <project.h>
}


using namespace mono;

ApplicationContext ApplicationContext::singleton;
IApplicationContext *IApplicationContext::Instance = NULL; //&(ApplicationContext::singleton);

ApplicationContext::ApplicationContext() : IApplicationContext(&pwrMgmt, &runLoop, &dispController), dispController()
{
    PWM_Start();
    PWM_WriteCompare2(0);
    PWM_WriteCompare1(0);
}

int ApplicationContext::exec()
{
    runLoop.exec();
    
    return 0;
}

void ApplicationContext::setMonoApplication(mono::IApplication *monoApp)
{
    this->application = monoApp;
    
    //CyIntSetSysVector(CY_INT_NMI_IRQN, &faultExceptionHandler);
    CyIntSetSysVector(CY_INT_HARD_FAULT_IRQN, &mbed_die);
    //CyIntSetSysVector(CY_INT_MEM_MANAGE_IRQN, &faultExceptionHandler);
    //CyIntSetSysVector(CY_INT_BUS_FAULT_IRQN, &faultExceptionHandler);
    //CyIntSetSysVector(CY_INT_USAGE_FAULT_IRQN, &faultExceptionHandler);
    
    //PowerSystem->onSystemPowerOnReset();
    pwrMgmt.processResetAwarenessQueue();
    
    //defaultSerial.printf("Display init deactivated\n\t");
    mono::IApplicationContext::Instance->DisplayController->init();
    
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
    
    PowerManager->EnterSleep();
    
    this->application->monoWakeFromSleep();
    
}

void ApplicationContext::_softwareReset()
{
    Bootloadable_SET_RUN_TYPE(0); // force bootloader to run for 2 secs
    CySoftwareReset();
    while(1); // silence compiler warning
}

void ApplicationContext::_softwareResetToBootloader()
{
    Bootloadable_SET_RUN_TYPE(Bootloadable_START_BTLDR);
    CySoftwareReset();
    while(1); // silence compiler warning
}

void ApplicationContext::resetOnUserButton()
{
    debug("Mono will reset on user button!\n\r");
    this->runLoop.setResetOnUserButton(true);
}

