#include "onewire.hpp"

using namespace mono::io;
using mono::String;

OneWire::OneWire
(
    PinName wire,
    uint32_t usPeriodLow_,
    uint32_t usPeriodHiZero_,
    uint32_t usPeriodHiOne_,
    // TODO: should be calculated from the two above and skipbits and buffer length.
    uint32_t usFullReadTimeOut_
) :
    state(Initialised),
    /// It is important that InterruptIn in initialised before...
    in(wire),
    /// ...DigitalOut because they use the same pin, and the initialisation
    /// sets the pin mode.
    out(wire,1,PullUp),
    usPeriodHiZero(usPeriodHiZero_),
    usPeriodHiOne(usPeriodHiOne_),
    usPeriodLow(usPeriodLow_),
    usFullReadTimeOut(usFullReadTimeOut_),
    bufferLength(0),
    usIntervalReadings(0)
{
}

void OneWire::restart (uint32_t usInitialPeriodLow, size_t skip, uint8_t * buf, size_t length)
{
    state = Writing;
    size_t oldLength = bufferLength;
    skipBits = skip;
    buffer = buf;
    bufferLength = length;
    if (bufferLength > oldLength)
        reallocateBitBuffer();
    startCommunicationByPullingLow(usInitialPeriodLow);
}

void OneWire::reallocateBitBuffer ()
{
    if (usIntervalReadings != 0)
        delete usIntervalReadings;
    usIntervalReadings = new uint32_t[maxBitsToRead()];
}

size_t OneWire::maxBitsToRead ()
{
    return skipBits + 8*bufferLength;
}

void OneWire::startCommunicationByPullingLow (uint32_t usInitialPeriodLow)
{
    clearBuffer();
    out = 0;
    ticker.attach_us(this,&OneWire::IRQ_letGoOfWireAndListen,usInitialPeriodLow);
}

void OneWire::clearBuffer ()
{
    memset(buffer,0,bufferLength);
    bitIndex = 0;
}

void OneWire::IRQ_letGoOfWireAndListen ()
{
    out = 1;
    state = Reading;
    usLastTimeStamp = us_ticker_read();
    in.fall(this,&OneWire::IRQ_falling);
    ticker.attach_us(this,&OneWire::IRQ_timeOut,usFullReadTimeOut);
}

void OneWire::IRQ_falling ()
{
    uint32_t usNow = us_ticker_read();
    usIntervalReadings[bitIndex] = usNow - usLastTimeStamp;
    usLastTimeStamp = usNow;
    ++bitIndex;
    if (bitIndex >= maxBitsToRead())
        IRQ_bufferFull();
}

void OneWire::IRQ_bufferFull ()
{
    ticker.detach();
    in.disable_irq();
    state = BufferFull;
    mono::async(this,&OneWire::collectBits);
}

void OneWire::IRQ_timeOut ()
{
    ticker.detach();
    in.disable_irq();
    state = TimeOut;
    mono::async(this,&OneWire::collectBits);
}

void OneWire::collectBits ()
{
    for (size_t i = skipBits; i < bitIndex; ++i)
    {
        uint32_t usHighPeriod = usIntervalReadings[i] - usPeriodLow;
        int const distanceZero = abs(usHighPeriod - usPeriodHiZero);
        int const distanceOne = abs(usHighPeriod - usPeriodHiOne);
        if (distanceOne < distanceZero)
            recordOneBitAt(i-skipBits);
        // Buffer is all-zeros initially so no need to set zero bits.
    }
    receiver.call();
}

void OneWire::recordOneBitAt (size_t index)
{
    size_t const byteIndex = index / 8;
    size_t byteBitIndex = 7 - (index % 8);
    buffer[byteIndex] |= (1 << byteBitIndex);
}
