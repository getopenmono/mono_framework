// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#if !defined(__com_openmono_dht_h)
#define __com_openmono_dht_h
#include <cmath>

namespace mono { namespace sensor { namespace dht {

enum SensorType { Unknown, DHT11, DHT22 };

#define BYTES_IN_READING 5

bool isProperReading (uint8_t const * data, size_t size)
{
    if (size != BYTES_IN_READING)
        return false;
    uint8_t const checksum = data[BYTES_IN_READING-1];
    uint8_t sum = 0;
    for (size_t i = 0; i < BYTES_IN_READING-1; ++i)
        sum += data[i];
    if (sum != checksum)
        return false;
    return true;
}

SensorType guessSensorType (uint8_t const * data, size_t size)
{
    if (size < 4 || size > 5)
        return Unknown;
    if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0)
        return Unknown;
    if (data[0] > 0 && data[1] == 0 && data[2] <= 100 && data[3] == 0)
        return DHT11;
    else if (data[0] < 4 && (data[2] == 0x80 || data[2] == 0x81 || data[2] < 4))
        return DHT22;
    return Unknown;
}

namespace dht22 {

float getTemperatureC (uint8_t const * data, size_t size)
{
    if (! isProperReading(data,size))
        return NAN;
    uint16_t abs = ((data[2]&0x7F)<<8) + data[3];
    bool negative = data[2]&0x80;
    if (negative)
        return abs / -10.0;
    return abs / 10.0;
}

float getHumidity (uint8_t const * data, size_t size)
{
    if (! isProperReading(data,size))
        return NAN;
    uint16_t abs = (data[0]<<8) + data[1];
    return abs / 10.0;
}

} // dht22

namespace dht11 {

float getTemperatureC (uint8_t const * data, size_t size)
{
    if (! isProperReading(data,size))
        return NAN;
    return data[2] + data[3] / 10.0;
}

float getHumidity (uint8_t const * data, size_t size)
{
    if (! isProperReading(data,size))
        return NAN;
    return data[0] + data[1] / 10.0;
}

} // dht11

} } } // mono - sensor - dht

#endif // __com_openmono_dht_h
