

#include "mn_serial.h"
#include "application_context_interface.h"

using namespace mono::io;

/// MARK: Private contructor

Serial::Serial(PinName tx, PinName rx) : mbed::Serial(tx, rx)
{
}

/// MARK: Public Constructor
Serial::Serial() : mbed::Serial(USBTX, USBRX)
{
    powersystem = IApplicationContext::Instance->PowerManager->PowerSystem;
}

bool Serial::DTR()
{
    return mbed::Serial::DTR();
}

bool Serial::IsReady()
{
    return powersystem->IsUSBCharging() && mbed::Serial::IsReady();
}

int Serial::vprintf(const char *format, va_list args)
{
    if (powersystem->IsUSBCharging())
    {
        return mbed::Serial::vprintf(format, args);
    }

    return 0;
}