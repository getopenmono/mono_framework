// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mono_battery_h
#define mono_battery_h

#include <stdint.h>

namespace mono { namespace power {

    /**
     * @brief Static class to get Mono's battery voltage or percentage
     *
     * @author Markus Laire
     */
    class MonoBattery
    {
    public:    
        static const uint16_t InitialDelay = 50;
        static const uint16_t SampleDelay = 10;
        static const uint16_t SampleCount = 64;

        static const uint32_t RawAdcMax = 0xFFF;
        static const uint32_t ReferenceVoltage = 0x400;
        static const uint32_t CorrectionFactor = 1588;
        static const uint32_t CorrectDenominator = 1000;
        static const uint32_t CorrectionOffset = 440;
        static const uint32_t CorrectionScale =
            RawAdcMax*CorrectionFactor/CorrectDenominator*ReferenceVoltage;

        static const uint16_t DefaultPercentageLookupTable[100];

        /**
         * @brief Calculate percentage of battery remaining
         * 
         * Percentage is calculated using a simple lookup-table of 100 voltage
         * values, in millivolts:
         * - if voltage < lookupTable[0], return 0
         * - else if voltage < lookupTable[1], return 1
         * - ...
         * - else if voltage < lookupTable[99], return 99
         * - else return 100
         * 
         * @param voltage Battery voltage as returned by ReadMilliVolts
         * @param lookupTable Custom lookup-table to use
         * @return Percentage of battery remaining
         */
        static uint8_t CalculatePercentage(uint16_t voltage,
            const uint16_t lookupTable[100] = DefaultPercentageLookupTable);

        /**
         * @brief Read the battery voltage in millivolts
         *
         * To get higher accuracy battery voltage is sampled multiple times
         * and average of these is returned.
         * 
         * Shortly after Mono has been reset, battery voltage can't be
         * read. In this case 0 is returned.
         *
         * @return The battery voltage in mV
         */
        static uint16_t ReadMilliVolts();

        /**
         * @brief Read the battery voltage and return percentage remaining
         *
         * See CalculatePercentage for details of how percentage is calculated.
         * 
         * @param lookupTable Custom lookup-table to use
         * @return Percentage of battery remaining
         */
        static uint8_t ReadPercentage(
            const uint16_t lookupTable[100] = DefaultPercentageLookupTable);
    };

} }

#endif /* mono_battery_h */
