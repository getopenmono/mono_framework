// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mono_power_aware_interface_h
#define mono_power_aware_interface_h

#include <deprecated.h>

namespace mono { namespace power {
    
    //forward declaration of PowerManagement interface
    class IPowerManagement;
    
    /**
     * @brief The Power Aware Abstract interface, for clases that handle power events.
     * 
     * Classes that implements this interface canreceive power related events in
     * from of handler methods. This interface define the 3 handler methods:
     *
     * 1. @ref `onSystemPowerOnReset` : Called reset or first power on
     * 2. @ref `onSystemEnterSleep` : Called just before system enter sleep mode
     * 3. @ref `onSystemWakeFromSleep` : Called right after the system has woken from sleep
     * 4. @ref `onSystemBatteryLow` : Called when battery is critically low
     *
     * Inheriting this interface is not enough to active the functionality. You
     * must remember to add your object instance to the @ref IPowerManagement 
     * instance, that can be obtained from the static  @ref IApplicationContext
     * class.
     */
    class IPowerAware
    {
        friend class IPowerManagement;
    public:
        
        /**
         * Next pointer in the power awareness object queue.
         * The @ref IPowerManagement uses this to traverse all power aware
         * objects.
         */
        IPowerAware *_pwrawr_nextPowerAware;
        
        /**
         * @brief Previous pointer in the power awareness queue.
         */
        IPowerAware *_pwrawr_previousPowerAware;
        
    
        /**
         * You can override this method. It gets called right after the system
         * power on for the first time, or after a reset condition.
         *
         * Use this method to setup / initialize components and data structures.
         * It is only called once for each system reset.
         *
         * @brief Called when the system powers on after a reset
         */
        virtual void onSystemPowerOnReset() { };
        
        /**
         * You can override this method to get sleep notifications. Before the
         * CPU stop executing intructions and goes into low power sleep mode,
         * this method gets called.
         *
         * Use this method to prepare your data or MCU components for sleep. Help
         * preserve battery by power down any peripheral, that is not needed
         * during sleep.
         *
         * This method can be called many times during your apps life cycle.
         * (That is between resets.) After each sleep period, when the MCU wakes
         * the @ref onSystemWakeFromSleep is guaranteed to be called.
         *
         * @brief Called right before the MCU goes into sleep mode
         */
        virtual void onSystemEnterSleep() { };
        
        /**
         * @brief Override to get notified when system wakes from sleep
         *
         * You can override this method to get wake-up notifications. When the
         * CPU starts executing intructions and the power system has powered up
         * all peripherals - this method gets called.
         *
         * Use this method to setup your app to resume after sleep mode.
         *
         * This method can be called many times during your apps life cycle.
         * (That is between resets.)
         */
        virtual void onSystemWakeFromSleep() { };
        
        /**
         * @brief Override to get notified when the battery voltage reaches a
         * critical level.
         *
         * You can override this method to get *battery low* notifications. When
         * this methods gets called, you have some time to finish critical tasks.
         * That might writing state to file system or transfer data over the
         * network.
         *
         * Depending on the health of the battery, the time between this
         * notification and the actual system enforced power off, might vary.
         *
         * In contrast to the other *power aware* methods this is only called
         * once in the application life cycle. After the enforced power off, when
         * the battery is charged, the system will automatically reset.
         */
        virtual void onSystemBatteryLow() { };
        virtual void OnSystemBatteryLow() __DEPRECATED("Capitalized method calls syntax is being obsoleted","onSystemBatteryLow") { onSystemBatteryLow(); }
    };
    
    
} }

#endif
