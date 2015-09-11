//
//  mono_power_management.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 09/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef mono__mono_power_management__
#define mono__mono_power_management__

#include "power_management_interface.h"
#include "act8600_power_system.h"

namespace mono { namespace power {
    
    class MonoPowerManagement : public IPowerManagement
    {
        
    public:
        
        void AppendToPowerAwareQueue(IPowerAware *object);
        bool RemoceFromPowerAwareQueue(IPowerAware *object);
        
    };
    
} }

#endif /* defined(__i2c_power_test__cy_power_management__) */
