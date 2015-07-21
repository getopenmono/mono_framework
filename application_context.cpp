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
}

int ApplicationContext::exec()
{
    return 0;
}

void ApplicationContext::setMonoApplication(mono::IApplication *monoApp)
{
    this->application = monoApp;
    mono::IApplicationContext::Instance->displayController->init();

    monoApp->monoWakeFromReset();
}
