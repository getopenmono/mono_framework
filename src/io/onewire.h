// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __com_openmono_onewire_h
#define __com_openmono_onewire_h

#include <mbed.h>
#include <mn_string.h>
#include <mn_digital_out.h>
#include <async.h>

namespace mono { namespace io {

    /**
     * A one-wire communication line with a sensor that has a data line
     * with pull-up.  The sensor is expected to be activated by pulling down the
     * wire and the releasing it.  The sensor is then expected to send data,
     * distinguishing between zero bits and one bits by two different period
     * lengths of logic-high on the wire.
     *
     * @author Jens Peter Secher
     */
    class OneWire
    {
        enum State { Initialised, Writing, Reading, BufferFull, TimeOut } state;
        mbed::InterruptIn in;
        mono::io::DigitalOut out;
        uint32_t usPeriodHiZero;
        uint32_t usPeriodHiOne;
        uint32_t usPeriodLow;
        size_t skipBits;
        uint32_t usFullReadTimeOut;
        uint8_t * buffer;
        size_t bufferLength;
        mbed::FunctionPointer receiver;
        size_t bitIndex;
        mbed::Ticker ticker;
        uint32_t usLastTimeStamp;
        uint32_t * usIntervalReadings;

        void clearBuffer ();
        void restart (uint32_t usInitialPeriodLow, size_t skipBits, uint8_t * buffer, size_t length);
        void startCommunicationByPullingLow (uint32_t usInitialPeriodLow);
        void read ();
        void attachTriggers ();
        void record (uint8_t bit);
        void collectBits ();
        void reallocateBitBuffer ();
        size_t maxBitsToRead ();
        void recordOneBitAt (size_t index);

        // Functions called in interrupts.
        void IRQ_letGoOfWireAndListen ();
        void IRQ_falling ();
        void IRQ_timeOut ();
        void IRQ_bufferFull ();

    public:

        /**
         * Setup a one-wire communication line with a sensor that has a data line
         * with pull-up.  The sensor is expected to be activated by pulling down the
         * wire and the releasing it.  The sensor is then expected to send data,
         * distinguishing between zero bits and one bits by two different period
         * lengths of logic-high on the wire.
         *
         * @param wire               Pin to communication through.
         * @param usPeriodLow        Expected logic-low length in µs common to all bits.
         * @param usPeriodHiZero     Expected logic-high length of a zero bit in µs.
         * @param usPeriodHiOne      Expected logic-high length of a one bit in µs.
         * @param usFullReadTimeOut  Timeout in µs for a complete read.
         */
        OneWire
        (
         PinName wire,
         uint32_t usPeriodLow,
         uint32_t usPeriodHiZero,
         uint32_t usPeriodHiOne,
         uint32_t usFullReadTimeOut
         );

        /**
         * Tell the sensor to start sending data.
         * @param usInitialPeriodLow  Period in µs that wire will be pulled low to initiate sensor.
         * @param skipBits            Number of initial handshake bits to ignore.
         * @param buffer              Buffer where the received bytes will be stored.
         * @param length              Buffer length in bytes.
         * @param obj                 Object that will recieve notification when read is done.
         * @param member              Object member that will recieve notification when read is done.
         *
         * Reading from the sensor stops when the buffer is filled up or when
         * timeout is reached, and then the buffer will contain whatever bits
         * that have been read so far.
         */
        template <typename Class>
        void send
        (
         uint32_t usInitialPeriodLow,
         size_t skipBits,
         uint8_t * buffer,
         size_t length,
         Class * obj,
         void (Class::*member) ()
         ) {
            receiver.attach<Class>(obj,member);
            restart(usInitialPeriodLow,skipBits,buffer,length);
        }
    };

} }

#endif // __com_openmono_onewire_h
