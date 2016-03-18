

#include "mn_serial.h"
#include "application_context_interface.h"
#include <us_ticker_api.h>
#include <device.h>

#ifdef DEVICE_SERIAL
#include <serial_usb_api.h>
#endif

using namespace mono::io;

/// MARK: Private contructor

Serial::Serial(PinName tx, PinName rx) : mbed::Serial(tx, rx)
{
}

/// MARK: Public Constructor
Serial::Serial() : mbed::Serial(USBTX, USBRX)
{
}

bool Serial::DTR()
{
#ifdef DEVICE_SERIAL
    if (serial_usbuart_init_cdc(&_serial))
    {
        return serial_usbuart_dtr(&_serial);
    }
#endif

    return true;
}

bool Serial::IsReady()
{
    return this->writeable();
}

