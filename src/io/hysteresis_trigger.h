
#ifndef hysteresis_trigger_h
#define hysteresis_trigger_h

#include <FunctionPointer.h>

namespace mono { namespace io {

    /**
     * @brief Hysteresis or Schmitt trigger function class
     *
     * This class implements a simple hysteresis check that allows you to 
     * continuously provide samples, that is checked against lower and upper
     * boundaries. The class will call your provided callback methods whenever
     * the hysteresis trigger changes from one boundary to another.
     *
     * A hysteresis trigger is a sound way of change program states, on base of
     * an analog signal value. This could be a light sensor or battery voltage
     * monitoring. A hysteresis loop will trigger the state changes on the base
     * signal value changes.
     *
     * ## Hysteresis
     *
     * This is a mechanism that defines an upper and lower threshold in signal
     * values, say 1000 and 100 for example. Whenever the input signal rises
     * above the upper threshold (1000 in this example), the hysteresis triggers
     * a statw change to an *upper* state.
     *
     * Low the signal value must fall below the lower threshold (100) to trigger
     * an equivalent *lower state* change. This process repeats, now the signal
     * must exceed the upper threshold again to trigger the *upper state* change.
     *
     * Read more on hyteresis triggers here: 
     * https://en.wikipedia.org/wiki/Hysteresis#Control_systems
     *
     * ## Example
     *
     * Here we setup a hysteresis trigger object, that triggers the callbacks
     * then a signal values exceed or fall under certia thresholds:
     *
     * @code
     * HysteresisTrigger sensorTrigger(10000, 3000);
     * 
     * sensorTrigger.setUpperTriggerCallback(this, &MyApp::clearErrorState);
     * sensorTrigger.setLowerTriggerCallback(this, &MyApp::setErrorState);
     *
     * // call this in a loop
     * uint16_t value = getSampleValue(); // get some signal value
     * sensorTrigger.check(value);
     * @endcode
     *
     * @see FilteredAnalogIn
     */
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

        /**
         * @brief Construct a new hysteresis object with specific boundaries
         *
         * Setup a hysteresis object with defined upper and lower threshold
         * values.
         *
         * @param upper The upper value (ceil) that triggers a state change
         * @param lower The lower value (floor) that triggers a state change
         * @param InitialState Optional: The start state of the hysteresis object
         */
        HysteresisTrigger(int upper, int lower, TriggerTypes InitialState = TRIG_LOWER_NEXT)
        {
            upperThreshold = upper;
            lowerThreshold = lower;
            nextTrigger = InitialState;
        }

        /**
         * @brief Provide a sample value to the hysteresis, that might trigger
         * a state change.
         *
         * Pass sample values of the signal source to this method, and it will
         * compared against the upper an lower thredshold of the hyteresis.
         *
         * If the `value` exceeds a threshold one of the state trigger hanlers
         * is called.
         *
         * @param value The signal sample to check against the threshold
         * @return `true` if an state changewas triggered, `false` otherwise
         */
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

        /**
         * @brief Return the current next state of the hyteresis loop
         *
         * This means if the signal was previously above the *upper threshold*
         * the next state change will be from *upper* to *lower*, eg. 
         * @ref TRIG_LOWER_NEXT
         */
        TriggerTypes NextTriggerType() const
        {
            return nextTrigger;
        }

        /**
         * @brief Set the hysteresis next state trigger
         *
         * You can force the hysteresis loop into a given state, by providing
         * its *next state* varaiable here.
         *
         * @param next The next state change that should be triggered
         */
        void setNextTrigger(TriggerTypes next)
        {
            nextTrigger = next;
        }

        /**
         * @brief Set the upper threshold callback
         *
         * Provide a method that gets called whenever the hysteresis loop
         * triggers for value above the *upper threshold*
         *
         * @param cnxt The context of the method to call, usually it is the `this`pointer.
         * @param memptr Function pointer to the method on the context class.
         */
        template <typename Context>
        void setUpperTriggerCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            upperHandler.attach<Context>(cnxt, memptr);
        }

        /**
         * @brief Set the lower threshold callback
         *
         * Provide a method that gets called whenever the hysteresis loop
         * triggers for value above the *lower threshold*
         *
         * @param cnxt The context of the method to call, usually it is the `this`pointer.
         * @param memptr Function pointer to the method on the context class.
         */
        template <typename Context>
        void setLowerTriggerCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            lowerHandler.attach<Context>(cnxt, memptr);
        }

    };
} }

#endif /* hysteresis_trigger_h */
