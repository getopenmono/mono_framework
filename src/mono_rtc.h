// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef mono_rtc_h
#define mono_rtc_h

#include <rtc_interface.h>

namespace mono {
    class MonoRTC : public IRTCSystem {
    public:
        
        void setupRtcSystem();


        void startRtc();


        void stopRtc();
    };
}

#endif /* mono_rtc_h */
