// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_battery.h"
#include <mbed.h>

using namespace mono::power;

const uint16_t MonoBattery::DefaultPercentageLookupTable[100] = {
    3379,3445,3473,3493,3511,3519,3524,3528,3532,3535,3538,3543,3547,3553,3558,
    3562,3569,3577,3584,3592,3599,3607,3614,3616,3618,3620,3622,3624,3626,3628,
    3629,3631,3632,3634,3636,3637,3639,3641,3644,3646,3648,3649,3652,3654,3657,
    3660,3662,3665,3668,3671,3674,3677,3680,3684,3686,3686,3686,3689,3694,3696,
    3696,3697,3702,3707,3712,3717,3722,3727,3733,3738,3743,3748,3754,3759,3767,
    3793,3807,3812,3818,3824,3829,3835,3841,3847,3854,3859,3866,3872,3878,3884,
    3890,3899,3915,3922,3928,3935,3941,3948,3957,3970
};

uint8_t MonoBattery::CalculatePercentage(uint16_t voltage, const uint16_t lookupTable[100])
{
    for (int percentage = 0; percentage < 100; percentage++) {
        if (voltage < lookupTable[percentage]) return percentage;
    }
    return 100;
}

uint16_t MonoBattery::ReadMilliVolts()
{
    static bool isStarted = false;
    if (! isStarted) {
        ADC_SAR_1_Start();
        isStarted = true;
    }

    // Disconnect AMUXBUSL ; Connect AG5 / CMP2 to AG5 / vref to CMP2
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW3) & ~0x01);
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW0, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW0) | 0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW4, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW4) | 0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW3) | 0x20);

    // wait for voltage level to settle
    wait_us(InitialDelay);

    int gotZero = false;
    uint32_t sum = 0;
    for (int n = 0; n < SampleCount; n++) {
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        uint16_t reading = ADC_SAR_1_GetResult16();
        
        // After reset value seems to stay at 0 for some time.
        // In that case don't try to calculate average, just return 0.
        if (reading == 0) {
            gotZero = true;
            break;
        }
        
        sum += reading;
        wait_us(SampleDelay);
    }

    // Disconnect CMP2 from vref and AG5 / AG5 from ADC ; Connect ADC to AMUXBUSL
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW4, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW4) & ~0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_CMP2_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_CMP2_SW3) & ~0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW0, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW0) & ~0x20);
    CY_SET_REG8(CYDEV_ANAIF_RT_SAR0_SW3, CY_GET_REG8(CYDEV_ANAIF_RT_SAR0_SW3) | 0x01);

    if (gotZero) {
        return 0;
    } else {
        uint32_t average = sum / SampleCount;
        // scale from 12 bit ADC to mV
        return CorrectionScale / average + CorrectionOffset;
    }
}

uint8_t MonoBattery::ReadPercentage(const uint16_t lookupTable[100])
{
    return CalculatePercentage(ReadMilliVolts(), lookupTable);
}
