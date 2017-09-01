// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mono_power_management_interface_h
#define mono_power_management_interface_h

#include "power_subsystem_interface.h"
#include "power_aware_interface.h"

#include <stddef.h>

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
        
        /**
         * Call all the power aware objects right after Power-On-Reset
         * The singleton power management object must call this method on reset
         */
        virtual void processResetAwarenessQueue()
        {
            if (powerAwarenessQueue == NULL)
                return;
            
            IPowerAware *task = powerAwarenessQueue;
            while (task != NULL) {
                
                task->onSystemPowerOnReset();
                task = task->_pwrawr_nextPowerAware;
            }
        }
        
        /**
         *
         * 
         */
        virtual void processSleepAwarenessQueue()
        {
            if (powerAwarenessQueue == NULL)
                return;
            
            IPowerAware *task = powerAwarenessQueue;
            while (task != NULL) {
                
                task->onSystemEnterSleep();
                task = task->_pwrawr_nextPowerAware;
            }
        }
        
        /**
         *
         * 
         */
        virtual void processWakeAwarenessQueue()
        {
            if (powerAwarenessQueue == NULL)
                return;
            
            IPowerAware *task = powerAwarenessQueue;
            while (task != NULL) {
                
                task->onSystemWakeFromSleep();
                task = task->_pwrawr_nextPowerAware;
            }
        }
        
        /**
         *
         * 
         */
        virtual void processBatteryLowAwarenessQueue()
        {
            if (powerAwarenessQueue == NULL)
                return;
            
            IPowerAware *task = powerAwarenessQueue;
            while (task != NULL) {
                
                task->onSystemBatteryLow();
                task = task->_pwrawr_nextPowerAware;
            }
        }
        
        
        /**
         *
         *
         */
        virtual void setupMCUPeripherals() {};
        
        /**
         *
         * 
         */
        virtual void powerDownMCUPeripherals() {}
        
        /**
         *
         * 
         */
        virtual void powerUpMCUPeripherals() {}
        
        
        virtual void removeObjectInPowerAwareQueue(IPowerAware *item)
        {
            if (item->_pwrawr_previousPowerAware != NULL)
                item->_pwrawr_previousPowerAware->_pwrawr_nextPowerAware = item->_pwrawr_nextPowerAware;
            
            if (item->_pwrawr_nextPowerAware != NULL)
                item->_pwrawr_nextPowerAware->_pwrawr_previousPowerAware = item->_pwrawr_previousPowerAware;
        }
        
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
         * @brief This variable must be `true` before sleep mode is aborted
         *
         * @ref EnterSleep() must set this to `false`, you should set it to `true`
         * to abort sleep mode and re-enter the run loop execution. @ref EnterSleep()
         * will not *return* before this is set to `true`.
         *
         * **Note: The class @ref QueuedInterrupt will set this automatically!**
         */
        static volatile bool __shouldWakeUp;

        /**
         * @brief Global flag to indicate to not halt CPU during sleep mode.
         * 
         * Some processes might require hardware to run async under what is
         * normally sleep mode, where hardware peripherals are halted.
         *
         * Set this flag to `true` to keep the the CPU awake inside the sleep
         * mode loop. This is especially useful when handled debouncing of
         * edge triggered interrupts.
         *
         * Note: You should keep this flag `false` to ensure power presevation
         */
        static volatile bool __busySleep;
        
        /**
         * @brief Send Mono to sleep mode, and stop CPU execution.
         * In sleep mode the CPU does not excute instruction and powers down into
         * a low power state.
         * The power system will turn off dynamically powered peripherals.
         *
         * Any power aware objects (@ref IPowerAware), that has registered itself 
         * in the @ref powerAwarenessQueuemust have its @ref onSystemEnterSleep 
         * method called.
         *
         * *NOTE: Before you call this method, make sure you have configured a way
         * to go out of sleep.*
         */
        virtual void EnterSleep() = 0;
        
        
        /**
         * @grief Add a @ref IPowerAware object to the awareness queue
         *
         * By added object to the Power Awareness Queue they receive callbacks
         * on power related events, such as reset and sleep.
         *
         * @param object A pointer to the object that is power aware
         */
        virtual void AppendToPowerAwareQueue(IPowerAware *object)
        {
            if (powerAwarenessQueue == NULL)
            {
                powerAwarenessQueue = object;
                object->_pwrawr_nextPowerAware = NULL;
                object->_pwrawr_previousPowerAware = NULL;
            }
            else
            {
                IPowerAware *item = powerAwarenessQueue;
                while (item->_pwrawr_nextPowerAware != NULL)
                {
                    item = item->_pwrawr_nextPowerAware;
                }
                
                item->_pwrawr_nextPowerAware = object;
                object->_pwrawr_previousPowerAware = item;
                object->_pwrawr_nextPowerAware = NULL;
            }
        }
        
        /**
         * Searches the Power Awareness Queue for the object and removes it, if
         * it is found. This object will no longer receive power related
         * notifications.
         *
         * @brief Remove an object from the Power Awareness Queue
         * @param object A pointer to the object that should be removed from the queue
         * @returns `true` if object was removed, `false` if the object was not in the queue
         */
        virtual bool RemoveFromPowerAwareQueue(IPowerAware *object)
        {
            if (powerAwarenessQueue == NULL)
                return false;
            
            IPowerAware *item = powerAwarenessQueue;
            
            while (item != NULL)
            {
                if (item == object)
                {
                    removeObjectInPowerAwareQueue(item);
                    return true;
                }
                
                item = item->_pwrawr_nextPowerAware;
            }
            
            return false;
        }
        
    };
    
} }

#endif
