// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mn_digital_out.h"

using namespace mono::io;

DigitalOut::DigitalOut(PinName pin) : mbed::DigitalOut(pin) { }

DigitalOut::DigitalOut(PinName pin, int value) : mbed::DigitalOut(pin, value) { }

DigitalOut::DigitalOut(PinName pin, int value, PinMode mode) : mbed::DigitalOut(pin, value)
{
    this->setMode(mode);
}

void DigitalOut::setMode(PinMode mode)
{
    gpio_mode(&this->gpio, mode);
}