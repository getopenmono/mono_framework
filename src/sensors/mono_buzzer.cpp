// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_buzzer.h"
#include <project.h>
#include "async.h"

#include "application_context.h"

using namespace mono::sensor;

MonoBuzzer::MonoBuzzer()
{
    buzzKill();
}

void MonoBuzzer::buzzKill()
{
    PWM_WriteCompare2(0);

    //stop any running timer
    timer.detach();
}

void MonoBuzzer::buzzAsync(uint32_t timeMs)
{
    PWM_WriteCompare2(dutyCycle);
    
    timer.attach_us<MonoBuzzer>(this, &MonoBuzzer::buzzTimeout, timeMs*1000);
}

void MonoBuzzer::buzzTimeout()
{
    buzzKill();
    
    if (timeoutHandler)
        async<MonoBuzzer>(this, &MonoBuzzer::asyncCall);
}

void MonoBuzzer::asyncCall()
{
    timeoutHandler.call();
    timeoutHandler.attach(0);
    timeoutHandler.attach<MonoBuzzer>(0, 0);
}
