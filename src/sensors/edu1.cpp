// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "edu1.h"
#include <mn_digital_out.h>
#include <application_context_interface.h>
#include <consoles.h>

using namespace mono::sensor;

Edu1::Edu1() :
    wire(J_RING1, DhtUsPeriodLow, DhtUsPeriodHiZero, DhtUsPeriodHiOne, DhtUsFullReadTimeout),
    timer(0)
{
    IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);

    setJackTip3V3();
    
    timer.setCallback<Edu1>(this, &Edu1::requestSensorReading);
    timer.start();
}

// MARK: Protected Methods

void Edu1::setJackTip3V3()
{
    io::DigitalOut(VAUX_EN,1);
    io::DigitalOut(VAUX_SEL,1);
    io::DigitalOut(JPO_nEN,0);
}

void Edu1::turnOffJackTip()
{
    io::DigitalOut(JPO_nEN,1);
}

void Edu1::readDone()
{
    timer.stop();
}

void Edu1::requestSensorReading()
{
    wire.send<Edu1>(DhtUsInitialPeriodLow, DhtUsSkipBits, buffer, 6, this, &Edu1::readDone);
}

// MARK: Public Methods

int Edu1::Read()
{
    return ReadMilliCelcius() / 1000;
}

int Edu1::ReadMilliCelcius()
{
    timer.start();
    dht::SensorType type = dht::guessSensorType(buffer, 5);
    if (type == dht::DHT11)
        return dht::dht11::getTemperatureC(buffer, 5) * 1000;
    else if (type == dht::DHT22)
        return dht::dht22::getTemperatureC(buffer, 5) * 1000;
    else {
        printf("unknown sensor!\r\n");
        return -1;
    }
}

void Edu1::onSystemEnterSleep()
{
    turnOffJackTip();
}

void Edu1::onSystemWakeFromSleep()
{
    setJackTip3V3();
}
