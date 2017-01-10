// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef rtc_interface_h
#define rtc_interface_h

namespace mono {

    /**
     * @brief Abstract interface for controlling a MCU specific RTC
     * 
     * This interface defines methods for setting up the RTC component, and
     * starting and stopping the RTC interrupt.
     *
     * The @ref DateTime class contains a static DateTime object that represents
     * the system time. It is this RTC interface's task to maintain a valid system
     * timestamp by incrementing the static system DateTime object.
     *
     */
    class IRTCSystem
    {
    public:
        
        /**
         * @brief The RTC should set this when it is time to process scheduled
         * tasks.
         *
         * The run loop and the sleep loop should check this variable to know
         * when to process all scheduled tasks. You must set this variable in
         * your RTC interrupt function.
         */
        static bool __shouldProcessScheduledTasks;

        /**
         * @brief Setup the RTC system with all register setting and interrupts
         * needed.
         *
         * This shuold not start the RTC, just intialize it - making it ready for
         * calling @ref startRtc(). 
         *
         * Any subclass must implement this function - it is automatically 
         * called by the @ref IApplicationContext class.
         */
        virtual void setupRtcSystem() = 0;


        /**
         * @brief Start the built-in RTC, effectively starting the system date/
         * time clock.
         */
        virtual void startRtc() = 0;


        /**
         * @brief Stop the built-in RTC, effectively pausing the system date/
         * time clock.
         */
        virtual void stopRtc() = 0;
    };

}

#endif /* rtc_interface_h */
