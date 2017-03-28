// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mono_buzzer_hpp
#define mono_buzzer_hpp

#include "buzzer_interface.h"
#include <mbed.h>

namespace mono { namespace sensor {

    class MonoBuzzer : public IBuzzer
    {
    protected:
        static const uint8_t dutyCycle = 6;
        mbed::Timeout timer;

        void buzzTimeout();
        
        void asyncCall();

    public:

        MonoBuzzer();

        void buzzAsync(uint32_t timeMs);
        void buzzKill();
    };

} }

#endif /* mono_buzzer_hpp */
