//
//  power_management_interface.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 09/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef i2c_power_test_power_management_interface_h
#define i2c_power_test_power_management_interface_h

#include "power_subsystem_interface.h"
#include "power_aware_interface.h"

namespace mono { namespace power {
    
    /**
     * @brief Generic abstract interface for the power management system
     *
     * A PowerManagement implementation class handles power related events and
     * sets up the system. The ApplicationContext object initializes an instance
     * of this class automatically. Use can find a pointer to the PowerManagement
     * object in the static @ref IApplicationContext class.
     *
     * Depending on the system (mono device or simulator), the concrete sub-class
     * implementation of this interface varies.
     *
     * The active @ref IApplicationContext initializes this class and calls its
     * POR initialization functions. Implementations of this class then calls 
     * and initializes any nessasary related classes, like the power supply IC
     * sub-system (@ref IPowerSubSystem).
     *
     * This interface defines queues of objects that implement the @ref
     * IPowerAware interface. This interface lets classes handle critical
     * power events like:
     *
     * * **Power-On-Reset** (POR): Called when the chip powers up after a reset
     * * **Enter Sleep Mode**: Called before system goes in to low power sleep mode
     * * **Awake From Sleep**: Called when nornal mode is restored after sleep
     *
     * #### Power Awareness Queue
     * Classes that handle components like I2C, Display, etc can use the PowerAware
     * interface to receive these type of events. Its the PowerManagement object
     * task to call these @ref IPowerAware objects.
     * 
     * The interface defines a protected member object @ref powerAwarenessQueue
     * that is a pointer to the first object in the queue. The Power Awareness
     * Queue is a list of objects that implment the @ref IpowerAware interface
     * and have added themselves to the queue by calling @ref AppendToPowerAwareQueue
     *
     * Objects in the queue receive power aware notifications on event like 
     * enter sleep mode, wake from sleep and battery low. You can add your own
     * objects to the queue to make them "power aware" or you remove the system
     * components that gets added by default. (But why would you do that?)
     */
    class IPowerManagement
    {
    protected:
        
        /**
         * @brief A pointer to the top object in the *Power Awareness Queue*
         *
         * The Power Awareness queue is realized by having the power object
         * themselves hold references to the next and previous object in the
         * queue. This eliminates the need for dynamic memory allocation a 
         * runtime.
         *
         * The @ref IPowerAware interface defines the *next* and *previous*
         * pointers for the object in the linked list. This class only holds a
         * reference to the first object in the queue.
         */
        IPowerAware *powerAwarenessQueue;
        
    public:
        
        
        /**
         * A pointer to the initialized power sub-system. This is initialize
         * automatically and depends on compiled environment.
         * The power system to used to control power supply to periphirals and
         * to give interrupt on power related events.
         *
         * **WARNING**: Use this class with extreme caution! Wrong power
         * settings can fry the MCU and other peripherals!
         */
        IPowerSubSystem *PowerSystem;
        
        /**
         * @grief Add a @ref IPowerAware object to the awareness queue
         *
         * By added object to the Power Awareness Queue they receive callbacks
         * on power related events, such as reset and sleep.
         *
         * @param object A pointer to the object that is power aware
         */
        virtual void AppendToPowerAwareQueue(IPowerAware *object) = 0;
        
        /**
         * Searches the Power Awareness Queue for the object and removes it, if
         * it is found. This object will no longer receive power related
         * notifications.
         *
         * @brief Remove an object from the Power Awareness Queue
         * @param object A pointer to the object that should be removed from the queue
         * @returns `true` if object was removed, `false` if the object was not in the queue
         */
        virtual bool RemoceFromPowerAwareQueue(IPowerAware *object) = 0;
        
    };
    
} }

#endif
