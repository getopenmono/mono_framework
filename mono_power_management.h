// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mono__mono_power_management__
#define mono__mono_power_management__

#include "power_management_interface.h"
#include "act8600_power_system.h"

namespace mono {
    class ApplicationContext; // forward decl.
}

namespace mono { namespace power {
    
    class MonoPowerManagement : public IPowerManagement
    {
        friend mono::ApplicationContext;
    protected:
        
        uint8_t DriveModeRegisters[3*9];
        
        ACT8600PowerSystem powerSubsystem;
        
        
        
        void processResetAwarenessQueue();
        
        void setupMCUPeripherals();
        void powerDownMCUPeripherals();
        void powerUpMCUPeripherals();
        
        void saveDMRegisters();
        void saveDMPort(uint32_t regAddrOffset, uint8_t *destOffset);
        void restoreDMRegisters();
        void restoreDMPort(uint32_t regAddrOffset, uint8_t *srcOffset);
        
    public:
        
        MonoPowerManagement();
        
        
        void EnterSleep();

        static bool __RTCFired;
    };
    
} }

#endif /* defined(__i2c_power_test__cy_power_management__) */
