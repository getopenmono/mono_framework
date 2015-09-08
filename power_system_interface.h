//
//  power_system_interface.h
//  mono
//
//  Created by Kristoffer Andersen on 08/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __i2c_power_test__power_system_interface__
#define __i2c_power_test__power_system_interface__

namespace mono { namespace power {
    
    /**
     * Abstract interface for the power sub-system. It defines 3 basic methods
     * related to reset, enter sleep and exit sleep modes.
     *
     * This interface is su-classed by impolementations of the different power
     * IC's on mono or an emulator.
     *
     * Subclasses of this interface should only conduct routines related to a
     * power sub-system - not to any CPU specific operations!
     */
    class IPowerSystem
    {
    public:
        
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
        
    };
    
} }

#endif /* defined(__i2c_power_test__power_system_interface__) */
