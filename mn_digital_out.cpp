//
//  mn_output_out.cpp
//  
//
//  Created by Kristoffer Andersen on 10/12/2015.
//  Copyright © 2015 your name. All rights reserved.
//

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