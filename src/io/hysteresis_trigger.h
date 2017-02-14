
#ifndef hysteresis_trigger_h
#define hysteresis_trigger_h

#include <FunctionPointer.h>

namespace mono { namespace io {

    class HysteresisTrigger
    {
    public:
        enum TriggerTypes
        {
            TRIG_LOWER_NEXT,
            TRIG_UPPER_NEXT
        };

    protected:

        int lowerThreshold, upperThreshold;
        TriggerTypes nextTrigger;
        mbed::FunctionPointer upperHandler, lowerHandler;


    public:

        HysteresisTrigger(int upper, int lower, TriggerTypes InitialState = TRIG_LOWER_NEXT)
        {
            upperThreshold = upper;
            lowerThreshold = lower;
            nextTrigger = InitialState;
        }

        bool check(int value)
        {
            if (value < lowerThreshold && nextTrigger == TRIG_LOWER_NEXT)
            {
                nextTrigger = TRIG_UPPER_NEXT;
                lowerHandler.call();
                return true;
            }
            else if (value > upperThreshold && nextTrigger == TRIG_UPPER_NEXT)
            {
                nextTrigger = TRIG_LOWER_NEXT;
                upperHandler.call();
                return true;
            }
            else
                return false;
        }

        TriggerTypes NextTriggerType() const
        {
            return nextTrigger;
        }

        void setNextTrigger(TriggerTypes next)
        {
            nextTrigger = next;
        }

        template <typename Context>
        void setUpperTriggerCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            upperHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setLowerTriggerCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            lowerHandler.attach<Context>(cnxt, memptr);
        }

    };
} }

#endif /* hysteresis_trigger_h */
