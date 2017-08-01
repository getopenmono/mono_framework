// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "catch.hpp"
#include "../sensors/dht.h"

using namespace mono::sensor;

#define BYTES_IN_READING 5

uint8_t badChecksumReading[BYTES_IN_READING] = { 0x02, 0x22, 0x00, 0xE2, 0x05 };

uint8_t noReading[BYTES_IN_READING] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t dht11Reading[BYTES_IN_READING] = { 0x24, 0x00, 0x16, 0x00, 0x3A };

uint8_t dht22PositiveTemperatureReading[BYTES_IN_READING] = { 0x01, 0xE7, 0x00, 0xE6, 0xCE };
uint8_t dht22NegativeTemperatureReading[BYTES_IN_READING] = { 0x02, 0x12, 0x80, 0x10, 0xA4 };
uint8_t dht22PositiveHighTemperatureReading[BYTES_IN_READING] = { 0x01, 0xE8, 0x01, 0x02, 0xEC };
uint8_t dht22PositiveHighHumidityReading[BYTES_IN_READING] = { 0x00, 0xFF, 0x00, 0x65, 0x64 };

TEST_CASE("DHT common")
{
    SECTION("reject short reading")
    {
        REQUIRE( ! dht::isProperReading(dht22PositiveTemperatureReading,4) );
    }
    SECTION("reject bad checksum")
    {
        REQUIRE( ! dht::isProperReading(badChecksumReading,BYTES_IN_READING) );
    }
    SECTION("verify good reading")
    {
        REQUIRE( dht::isProperReading(dht11Reading,BYTES_IN_READING) );
        REQUIRE( dht::isProperReading(dht22PositiveTemperatureReading,BYTES_IN_READING) );
        REQUIRE( dht::isProperReading(dht22NegativeTemperatureReading,BYTES_IN_READING) );
        REQUIRE( dht::isProperReading(dht22PositiveHighTemperatureReading,BYTES_IN_READING) );
        REQUIRE( dht::isProperReading(dht22PositiveHighHumidityReading,BYTES_IN_READING) );
    }
    SECTION("Detect DHT11 sensor")
    {
        REQUIRE( dht::guessSensorType(dht11Reading,BYTES_IN_READING) == dht::DHT11 );
    }
    SECTION("Detect DHT22 sensor")
    {
        REQUIRE( dht::guessSensorType(dht22NegativeTemperatureReading,BYTES_IN_READING) == dht::DHT22 );
        REQUIRE( dht::guessSensorType(dht22PositiveHighTemperatureReading,BYTES_IN_READING) == dht::DHT22 );
        REQUIRE( dht::guessSensorType(dht22PositiveHighHumidityReading,BYTES_IN_READING) == dht::DHT22 );
    }
    SECTION("No reading")
    {
        REQUIRE( dht::guessSensorType(0,0) == dht::Unknown );
        REQUIRE( dht::guessSensorType(noReading,BYTES_IN_READING) == dht::Unknown );
    }
}
TEST_CASE("DHT11")
{
    SECTION("detect bad reading")
    {
        REQUIRE( isnan(dht::dht11::getTemperatureC(badChecksumReading,BYTES_IN_READING)) );
    }
    SECTION("good temperature reading")
    {
        REQUIRE( dht::dht11::getTemperatureC(dht11Reading,BYTES_IN_READING) == 22.0f );
    }
    SECTION("good humidity reading")
    {
        REQUIRE( dht::dht11::getHumidity(dht11Reading,BYTES_IN_READING) == 36.0f );
    }
}
TEST_CASE("DHT22")
{
    SECTION("detect bad reading")
    {
        REQUIRE( isnan(dht::dht22::getTemperatureC(badChecksumReading,BYTES_IN_READING)) );
    }
    SECTION("positive temperature")
    {
        REQUIRE( dht::dht22::getTemperatureC(dht22PositiveTemperatureReading,BYTES_IN_READING) == 23.0f );
    }
    SECTION("negative temperature")
    {
        REQUIRE( dht::dht22::getTemperatureC(dht22NegativeTemperatureReading,BYTES_IN_READING) == -1.6f );
    }
    SECTION("positive high temperature")
    {
        REQUIRE( dht::dht22::getTemperatureC(dht22PositiveHighTemperatureReading,BYTES_IN_READING) == 25.8f );
    }
    SECTION("humidity")
    {
        REQUIRE( dht::dht22::getHumidity(dht22PositiveTemperatureReading,BYTES_IN_READING) == 48.7f );
    }
    SECTION("high humidity")
    {
        REQUIRE( dht::dht22::getHumidity(dht22PositiveHighHumidityReading,BYTES_IN_READING) == 25.5f );
    }
}
