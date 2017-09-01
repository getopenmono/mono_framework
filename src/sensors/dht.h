// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef __com_openmono_dht_h
#define __com_openmono_dht_h

#include <stdint.h>
#include <stdio.h>

namespace mono { namespace sensor { namespace dht {

    enum SensorType { Unknown, DHT11, DHT22 };

    bool isProperReading (uint8_t const * data, size_t size);

    SensorType guessSensorType (uint8_t const * data, size_t size);

    namespace dht22 {

        float getTemperatureC (uint8_t const * data, size_t size);

        float getHumidity (uint8_t const * data, size_t size);

    } // dht22

    namespace dht11 {

        float getTemperatureC (uint8_t const * data, size_t size);

        float getHumidity (uint8_t const * data, size_t size);

    } // dht11

} } } // mono - sensor - dht

#endif // __com_openmono_dht_h
