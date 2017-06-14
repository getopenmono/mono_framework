// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef psoc_battery_voltage_h
#define psoc_battery_voltage_h

#include <stdint.h>

namespace mono { namespace power {

    class BatteryVoltage
    {
    public:

        static const uint32_t RawAdcMax = 0xFFF;
        static const uint32_t ReferenceVoltage = 0x400;
        static const uint32_t CorrectionFactor = 1588;
        static const uint32_t CorrectDenominator = 1000;
        static const uint32_t CorrectionOffset = 440;

        static const uint32_t CorrectionScale =
        RawAdcMax*CorrectionFactor/CorrectDenominator*ReferenceVoltage;

        static uint16_t mVoltage();
    };

} }

#endif /* psoc_battery_voltage_h */
