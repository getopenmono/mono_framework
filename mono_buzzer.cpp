// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_buzzer.h"
#include <project.h>

using namespace mono::sensor;

mono::Timer *MonoBuzzer::bTimer = 0;

MonoBuzzer::MonoBuzzer()
{
    buzzKill();
}

void MonoBuzzer::buzzKill()
{
    PWM_WriteCompare2(0);

    if (bTimer != 0)
    {
        bTimer->Stop();
        bTimer = 0;
    }

}

void MonoBuzzer::buzzAsync(uint32_t timeMs)
{
    if (bTimer != 0)
    {
        bTimer->Stop();
    }

    PWM_WriteCompare2(dutyCycle);
    bTimer = Timer::callOnce<MonoBuzzer>(timeMs, this, &MonoBuzzer::buzzKill);
}