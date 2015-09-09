//
//  power_aware_interface.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 09/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef mono_power_aware_interface_h
#define mono_power_aware_interface_h

namespace mono { namespace power {
    
    //forward declaration of PowerManagement interface
    class IPowerManagement;
    
    /**
     * @brief The Power Aware Abstract interface, for clases that handle power events.
     * 
     * Classes that implements this interface canreceive power related events in
     * from of handler methods. This interface define the 3 handler methods:
     *
     * 1. `@ref onSystemPowerOnReset` : Called reset or first power on
     * 2. `@ref onSystemEnterSleep` : Called just before system enter sleep mode
     * 3. `@ref onSystemWakeFromSleep` : Called right after the system has woken from sleep
     * 4. `@ref OnSystemBatteryLow` : Called when battery is critically low
     *
     * Inheriting this interface is not enough to active the functionality. You
     * must remember to add your object instance to the @ref IPowerManagement 
     * instance, that can be obtained from the static  @ref IApplicationContext
     * class.
     */
    class IPowerAware
    {
        friend class IPowerManagement;
    private:
        
        /**
         * Next pointer in the power awareness object queue.
         * The @ref IPowerManagement uses this to traverse all power aware
         * objects.
         */
        IPowerAware *_pwrawr_nextPowerAware;
        
        /**
         * Previous pointer in the power awareness queue.
         */
        IPowerAware *_pwrawr_previousPowerAware;
        
    protected:
        /**
         * You must override this method. It gets called right after the system
         * power on for the first time, or after a reset condition.
         *
         * Use this method to setup / initialize components and data structures.
         * It is only called once for each system reset.
         *
         * @brief Called when the system powers on after a reset
         */
        virtual void onSystemPowerOnReset() = 0;
        
        /**
         * You must override this method to get sleep notifications. Before the
         * CPU stop executing intructions and goes into low power sleep mode,
         * this method gets called.
         *
         * Use this method to prepare your data or MCU components for sleep. Help
         * preserve battery by power down any peripheral, that is not needed
         * during sleep.
         *
         * This method can be called many times during the firmware life cycle.
         * (That is between resets.) After each sleep period, when the MCU wakes
         * the @ref onSystemWakeFromSleep is guaranteed to be called.
         *
         * @brief Called right before the MCU goes into sleep mode
         */
        virtual void onSystemEnterSleep() = 0;
        
        /**
         * <# description #>
         *
         * @brief <# brief desc #>
         * @param <# param desc #>
         * @param <# param desc #>
         * @returns <# return desc #>
         */
        virtual void onSystemWakeFromSleep() = 0;
        
        
        virtual void OnSystemBatteryLow() = 0;
    };
    
    
} }

#endif
