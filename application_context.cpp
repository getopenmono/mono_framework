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
IApplicationContext *IApplicationContext::Instance = NULL; //&(ApplicationContext::singleton);

ApplicationContext::ApplicationContext() : IApplicationContext(&pwrMgmt, &runLoop, &dispController), dispController()
{
    PWM_Start();
    PWM_WriteCompare1(0);
    PWM_WriteCompare2(0);
    
    //CyIntSetSysVector(<#uint8 number#>, <#cyisraddress address#>)
}

int ApplicationContext::exec()
{
    runLoop.exec();
    
    return 0;
}

void ApplicationContext::setMonoApplication(mono::IApplication *monoApp)
{
    this->application = monoApp;
    //PowerSystem->onSystemPowerOnReset();
    //pwrMgmt.processResetAwarenessQueue();
    
    defaultSerial.printf("Display init deactivated\n\t");
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


void faultExceptionHandler()
{
    while(1)
    {
        PWM_WriteCompare1(0);
        wait(0.5);
        PWM_WriteCompare1(64);
        wait(0.25);
    }
}