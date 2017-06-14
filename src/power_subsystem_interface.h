// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __i2c_power_test__power_subsystem_interface__
#define __i2c_power_test__power_subsystem_interface__

#include <FunctionPointer.h>

namespace mono { namespace power {
    
    /**
     * Abstract interface for the power sub-system. It defines 3 basic methods
     * related to reset, enter sleep and exit sleep modes.
     *
     * This interface is sub-classed by implementations of the different power
     * supply IC's on mono or an emulator.
     *
     * Subclasses of this interface should only conduct routines related to a
     * power sub-system - not to any CPU specific operations! This means setting
     * up voltage levels and enabling power fencing to peripherals.
     *
     * This power supply sub-system interface also defines callbacks that are
     * called then the battery events occur. These are:
     *
     * * Battery low warning
     *
     * You can listen to these events by supplying a callback handler function
     *
     * Power to MCU internal modules are controller by the abstract interface
     * for power management @ref IPowerManagement
     */
    class IPowerSubSystem
    {
    public:

        /** 
         * Battery charing states
         * @see ChargeStatus
         */
        enum ChargeState
        {
            UNKNOWN,                /**< Chip does not support charging or dont disclore it */
            CHARGE_PRECONDITION,    /**< Charging has just begun (pre-condition) */
            CHARGE_FAST,            /**< Fast Charging in constant current mode */
            CHARGE_SLOW,            /**< Slower charging, in Constant Voltage mode */
            CHARGE_ENDED,           /**< Charge ended of cycle, battery is full */
            CHARGE_SUSPENDED        /**< No battery attached or wrong battery voltage levels */
        };
        
        /**
         * Called by the application as first thing after power-on or system
         * reset.
         *
         * The function must set up the default power configuration of the system,
         * peripherals, voltages etc.
         */
        virtual void onSystemPowerOnReset() = 0;
        
        /**
         * Called before the system enter a sleep mode, where the CPU is not
         * excuting instructions. To enable the lowest possible power consumption
         * subclasses can turn off selected periphrals here.
         */
        virtual void onSystemEnterSleep() = 0;
        
        /**
         * Called after the system has woken from a sleep mode. This is only
         * called after an call to @ref onSystemEnterSleep has occured.
         * Use this method to turn on any disabled peripheral.
         */
        virtual void onSystemWakeFromSleep() = 0;

        /**
         * @brief Return the current status of the Power Fence
         * 
         * The power fence cuts power to specific peripherals. Each peripheral 
         * driver should know whether or not it is behind the fence.
         *
         * @return `true` if the power fence is active (power is not present), `false` if power is ON.
         */
        virtual bool IsPowerFenced() { return false; }

        /**
         * @brief Turn on/off the power fence
         *
         * Some peripherals are behind a power fence, that can cut their power.
         * You can control this power, and remove their supply upon going to
         * sleep mode, to safe battery.
         *
         * @param active `true` will cut the power, `false` will power the peripherals
         */
        virtual void setPowerFence(bool) {}

        /**
         * @brief Get the current charge status for the attached battery
         * 
         * The Subsystem implememtation might be able to monitor the current
         * charging state of the battery. If no battery exists the state will be
         * @ref SUSPENDED. If the implementation does not support charge states
         * this method will always return @ref UNKNOWN.
         *
         * The different states is explained by the @ref ChargeState enum.
         *
         * @return The current charge state integer
         * @see ChargeState
         */
        virtual ChargeState ChargeStatus() { return UNKNOWN; }


        /**
         * @brief Get the USB charging state (True if charging now)
         * 
         * This methods default implementation uses the @ref ChargeStatus method
         * to check the CHARGE_* enum and `true` if it is not SUSPENDED or 
         * UNKNOWN.
         *
         * PowerSubsystem subclasses might override this method do their own checks.
         */
        virtual bool IsUSBCharging()
        {
            ChargeState state = this->ChargeStatus();
            return state != CHARGE_SUSPENDED && state != UNKNOWN;
        }

        /**
         * @brief Return `true` is the battery voltage is OK, `false` is empty.
         * 
         * This method query the system power state, to see if the battery is OK.
         * **In case this return `false`, the system should enter low-power 
         * sleep immediately!**
         */
        virtual bool IsPowerOk() = 0;

        /** Function handler that must be called when the PowerSystem detect low battery */
        mbed::FunctionPointer BatteryLowHandler;

        /** Function handler that must be called when battery is empty */
        mbed::FunctionPointer BatteryEmptyHandler;

    };
    
} }

#endif /* defined(__i2c_power_test__power_system_interface__) */
