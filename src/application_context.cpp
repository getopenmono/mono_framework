// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

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

ApplicationContext::ApplicationContext() : IApplicationContext(
                                                               &pwrMgmt,
                                                               &runLoop,
                                                               &dispController,
                                                               &touchSys,
                                                               &UserButton,
                                                               &at30ts64Sensor,
                                                               &mmaAccelerometer,
                                                               &mnBuzzer,
                                                               &rtc),

                                            dispController(),
                                            UserButton(SW_USER, PullUp)
{
    // Start the PWM component (as required by buzzer and TFT)
    PWM_Start();

    //setup default user button handler
    //UserButton.DeactivateUntilHandled(); // dont queue up pushes
    UserButton.setDebouncing(true);
    UserButton.setInterruptsSleep(true);
    UserButton.fall<ApplicationContext>(this, &ApplicationContext::enterSleepMode);

    printf("\r\n");
}

int ApplicationContext::exec()
{
    runLoop.exec();

    return 0;
}

void ApplicationContext::setMonoApplication(mono::IApplication *monoApp)
{
    this->application = monoApp;

    CyIntSetSysVector(CY_INT_NMI_IRQN, &mbed_die);
    CyIntSetSysVector(CY_INT_HARD_FAULT_IRQN, &mbed_die);
    CyIntSetSysVector(CY_INT_MEM_MANAGE_IRQN, &mbed_die);
    CyIntSetSysVector(CY_INT_BUS_FAULT_IRQN, &mbed_die);
    CyIntSetSysVector(CY_INT_USAGE_FAULT_IRQN, &mbed_die);

    //PowerSystem->onSystemPowerOnReset();
    pwrMgmt.processResetAwarenessQueue();

    //defaultSerial.printf("Display init deactivated\n\t");
    mono::IApplicationContext::Instance->DisplayController->init();

    touchSys.init();
    
    rtc.setupRtcSystem();
    rtc.startRtc();

    monoApp->monoWakeFromReset();

}

void ApplicationContext::sleepForMs(uint32_t ms)
{
    uint8_t ticks = 1;
    while ((1u << ticks) < ms) {
        ticks++;
    }

    defaultSerial.printf("Setting CTW to: %i for ms: %i\r\n",ticks, ms);
    *((reg8*)CYREG_PM_TW_CFG1) = 0x0F & ticks;
    PM_TW_CFG2 = PM_TW_CFG2 | 0x0C; // CTW enable and CTW interrupts
    defaultSerial.printf("CTW CFG Reg: 0x%x\r\n",PM_TW_CFG2);
    enterSleepMode();
}

void ApplicationContext::enterSleepMode()
{
    UserButton.fall<ApplicationContext>(this, &ApplicationContext::buttonWakeup);
    
    this->application->monoWillGotoSleep();

    PowerManager->EnterSleep();

    this->application->monoWakeFromSleep();

}

void ApplicationContext::buttonWakeup()
{
    UserButton.fall<ApplicationContext>(this, &ApplicationContext::enterSleepMode);
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

void ApplicationContext::_softwareResetToApplication()
{
    Bootloadable_SET_RUN_TYPE(Bootloadable_START_APP);
    CySoftwareReset();
    while(1); // silence compiler warning
}

void ApplicationContext::resetOnUserButton()
{
    debug("Mono will reset on user button!\r\n");
    this->runLoop.setResetOnUserButton(true);
}

