// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "psoc_battery_voltage.h"

#include <mbed.h>

extern "C" {
#include <project.h>
}

using namespace mono::power;



uint16_t BatteryVoltage::mVoltage()
{
    static bool isStarted = false;
    if (!isStarted) {
        ADC_SAR_1_Start();
        isStarted = true;
    }

    // Disconnect AMUXBUSL
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW3) & ~0x01);

    // Connect AG5
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW0, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW0) | 0x20);

    // Connect CMP2 to AG5
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW4, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW4) | 0x20);

    // Connect vref to CMP2
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW3) | 0x20);

    // wait for voltage level to settle
    wait_us(20);

    ADC_SAR_1_StartConvert();
    ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);

    // disconnect CMP2 from vref and AG5, AG5 from ADC
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW4, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW4) & ~0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW3) & ~0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW0, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW0) & ~0x20);

    // connect ADC to AMUXBUSL
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW3) | 0x01);

    return CorrectionScale/ADC_SAR_1_GetResult16() + CorrectionOffset; // scale from 12 bit ADC to mV
}
