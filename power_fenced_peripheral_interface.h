//
//  power_fenced_peripheral_interface.h
//  mono framework
//
//  Created by Kristoffer Andersen on 06/01/2016.
//  Copyright © 2016 your name. All rights reserved.
//

#ifndef power_fenced_peripheral_interface_h
#define power_fenced_peripheral_interface_h

#include "queue.h"

namespace mono { namespace power {
    
    /**
     * @brief Interface for hardware controller behind the power fence
     * 
     */
    class IPowerFencedPeripheral : public IQueueItem
    {
    protected:
        
        bool _needs_power;
        
    public:
        
        /**
         * @brief Construct the base interface for hardware controllers that 
         * depends on the power fence system.
         * 
         * This will add the object to the callback chain
         */
        IPowerFencedPeripheral()
        {
            
        }
        
        /// mark: Methods to override
        
        /**
         * @brief Callback for notification of power going down (hardware being)
         * power fenced.
         * 
         * You _must_ override this virtual method, to handle when power is fenced
         *
         * After this method return expect power to the hardware is cut.
         */
        virtual void PowerGoesDown() = 0;
        
        /**
         * @brief Callback for notification of power has just came up.
         * 
         * You can override this method, if you need notifications for power becoming
         * available. Alternatively you can check the @ref HasPower method.
         */
        virtual void PowerCameUp() {};
        
        /// mark: Standard implemented methods
        
        /**
         * @brief Returns `true` if the power is avialable
         * 
         */
        bool HasPower() const
        {
            return false;
        }
        
        /**
         * @brief Reuest the power manager to bring up the power. Returns `true`
         * if power is available.
         * 
         */
        bool RequestPower()
        {
            return false;
        }
        
        /**
         * @brief Tell the Power Manager that you don't need power anymore.
         *
         * If all fenced peripherals release there power requirement, the power is
         * cut.
         *
         */
        bool ReleasePower()
        {
            return false;
        }
        
        /**
         * @brief Force the power manager to turn off and on the power. This will
         * reset all fenced periphrals.
         * 
         * ** Do not use this for fun**
         */
        bool ForceTogglePower()
        {
            return false;
        }
        
    };
    
} }


#endif /* power_fenced_peripheral_interface_h */
