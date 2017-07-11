
#ifndef _edu1_h
#define _edu1_h

#include <temperature_interface.h>
#include <onewire.h>
#include <dht.hpp>
#include <FunctionPointer.h>
#include <power_aware_interface.h>

namespace mono { namespace sensor {

    class Edu1 : public ITemperature, public power::IPowerAware
    {
        static const int DhtUsPeriodLow = 50;
        static const int DhtUsPeriodHiZero = 27;
        static const int DhtUsPeriodHiOne = 70;
        static const int DhtUsFullReadTimeout = 50+80+5*8*(50+70);
        static const int DhtUsInitialPeriodLow = 18*1000;
        static const int DhtUsSkipBits = 2;

    protected:

        mbed::FunctionPointer btnTrig;
        mono::io::OneWire wire;
        uint8_t buffer[6];
        Timer timer;

        void setJackTip3V3();
        void turnOffJackTip();
        void readDone();
        void requestSensorReading();

    public:

        Edu1();

        int Read();
        int ReadMilliCelcius();

        void setButtonCallback(void(*funcPtr)())
        {
            btnTrig.attach(funcPtr);
        }

        template <typename Class>
        void setButtonCallback(Class *obj, void(Class::*memptr)(void))
        {
            btnTrig.attach<Class>(obj, memptr);
        }

        void onSystemEnterSleep();
        void onSystemWakeFromSleep();
    };

} }

#endif /* edu1_h */
