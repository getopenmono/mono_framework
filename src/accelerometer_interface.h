// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef accelerometer_interface_h
#define accelerometer_interface_h

#include <stdint.h>

namespace mono { namespace sensor {

    /**
     * @brief Abstract inteface for interacting with the accelerometer
     *
     *
     */
    class IAccelerometer
    {
    public:

        /**
         * @brief Start the accelerometer
         *
         * Before you can sample any acceleration, you must start the
         * accelerometer. When the accelerometer is running its power consumption
         * will likely increase. Remember to @ref Stop the accelerometer, when
         * you are done sampling the acceleration.
         */
        virtual void Start() { start(); };
        virtual void start() = 0;

        /**
         * @brief Stops the accelerometer
         *
         * A stopped accelerometer can not sample acceleration. @ref Start the
         * accelerometer before you sample any axis.
         */
        virtual void Stop() { stop(); };
        virtual void stop() = 0;

        /**
         * @brief Return the current Start/Stop state of the accelerometer
         *
         * @returns `true` only if the accelerometer is started and sampling data
         */
        virtual bool IsActive() = 0;

        /**
         * @brief <# brief desc #>
         *
         * @returns <# return desc #>
         */
        virtual int16_t rawXAxis(bool monoOrientation = true) = 0;

        /**
         * @brief <# brief desc #>
         *
         * @returns <# return desc #>
         */
        virtual int16_t rawYAxis(bool monoOrientation = true) = 0;

        /**
         * @brief <# brief desc #>
         *
         * @returns <# return desc #>
         */
        virtual int16_t rawZAxis(bool monoOrientation = true) = 0;

    protected:

        // NOTE: Should we have these here?
#ifndef EMUNO
        virtual bool readRegister(uint8_t regAddr, uint8_t *data) = 0;
        virtual bool readRegister(uint8_t regAddr, uint16_t *data) = 0;

        virtual bool writeRegister(uint8_t regAddr, uint8_t data) = 0;
#endif
    };

} }

#endif /* accelerometer_interface_h */
