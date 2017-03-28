// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mn_serial_h
#define mn_serial_h

#include <Serial.h>
#include "power_subsystem_interface.h"

namespace mono { namespace io {

    /**
     * @brief Power / USB aware serial port class,  that uses only the USB UART
     * 
     * THis is the implementation of communication via USBUART. It builds upon
     * mbed's @ref mbed::Serial class. Because mbed ti not aware of changes power
     * in power and charging states, this class acts as a wrapper around the
     * `mbed::Serial` class.
     *
     * ## Restrictions
     * This Serial class will communicate with the USBUART only when the USB 
     * power is connected. All data consumed by the class, then the USB power is
     * absent, is ignored.
     *
     *
     */
    class Serial : public mbed::Serial
    {
    protected:
        power::IPowerSubSystem *powersystem;

        Serial(PinName tx, PinName rx);

    public:

        /**
         * @brief Create a new instance of the USB UART port
         * 
         * This is the only available contructor, because the RX and TX lines 
         * are hardwired. Ideally you should not initialize your own USB serial
         * port, but use the systems default I/O streams.
         */
        Serial();

        /**
         * @brief See the status for the Data Terminal Ready signal
         * @return `true` if DTR is set
         */
        bool DTR();

        /**
         * @brief Test if the USB is connected (powered) and mounted CDC device
         * is a CDC device by the host.
         * 
         * Before any data can be sent via the USB Serial link, the host (computer)
         * must have emunerated Mono as a USB CDC device. ALso, to ensure the USB
         * cable is connected, the USB power is monitored. These two conditions
         * must be met, before this method returns `true`.
         *
         * @return `true` if USB power is on and USB is enumerated on host
         */
        bool IsReady();

        
    };

} }

#endif /* mn_serial_h */
