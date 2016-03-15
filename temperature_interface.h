// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef temperature_interface_h
#define temperature_interface_h

namespace mono { namespace sensor {

    /**
     * @brief Abstract Interface for temperature sensors
     *
     * This interface creates a hardware-independent abstraction layer for
     * interacting with temperature sensors.  Hardware temperature sensors
     * in Mono must subclass this interface.
     *
     * To access the temperature sensor, use the global object:
     *
     * @code
     *  ITemperature *mono::sensor::Temperature
     * @endcode
     *
     * This object is automatically be initialized by the @ref IApplicationContext
     * and the current `ITemperature` subclass.
     */
    class ITemperature
    {
    public:

        /**
         * Reads the current temperature from the temperature sensor
         * @return the temperature in Celcius
         */
        virtual int Read() = 0;

        /**
         * @brief Reads the temperature in fixed point milli-Celcius
         *
         * To get a higher precision output, this method will return milliCelcius
         * such that: 22,5 Celcius == 22500 mCelcius
         *
         * @return The temperature in mCelcius
         */
        virtual int ReadMilliCelcius() {return Read() * 1000; }

    };

} }

#endif /* temperature_interface_h */
