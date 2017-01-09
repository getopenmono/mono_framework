
#ifndef power_saver_h
#define power_saver_h

#include <mono.h>

namespace mono {
    
    class PowerSaver : public TouchResponder
    {
    protected:
        Timer dimTimer, sleepTimer;
        int dimBright;
        int fullBright;
        void dimStep();
        
    public:
        
        PowerSaver(int dimTimeoutMs = 3000, int sleepTimeoutMs = 10000, int dimBrightness = 30, int fullBrightness = 255);
        
        void startDimTimer();
        void startSleepTimer();
        void dim();
        void undim();
        void deactivate();
        
        virtual void RespondTouchBegin(TouchEvent &event);
    };
}

#endif /* power_saver_h */
