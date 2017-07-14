
#ifndef power_saver_h
#define power_saver_h

#include <mono.h>

namespace mono {

    /**
     * @brief Auto dim the display and sleep Mono after a given period of inactivity.
     * 
     * This class will automatically dim the display and enter sleep mode,
     * after a period of user inactivity. The uses 3 states:
     *
     * 1. Normal mode (full brightness on display)
     * 2. After a period of no user touch input (default 10 secs.),
     *    display dims to 11%
     * 3. After yet a period (default  10 secs.) of no touch input, sleep
     *    mode is entered.
     *
     * Any touch input will reset the state to *1.*
     *
     * You should add this objact to your *AppController* class, to enable
     * automatic power saving.
     *
     * #### Multiple PowerSavers
     *
     * If you want to use multiple instances of this class, remember to @ref
     * deactivate the instances that should be inactive. Having two active
     * instances at one time, can be confusing to the users.
     *
     * #### Catch-all Touches
     *
     * TO capture all touches, any instane should be the first to respond to
     * touch inputs. The touch system has a responder chain of objects that
     * handles touch input. Any of these objects might choose to break the chain
     * and allow no further processing of a given touch event.
     *
     * Therefore the PowerSaver must be the first (or one of) to handle touch 
     * events. You accomplish this by initializing the PowerSaver object as the
     * first member object of your AppController. Before any other touch related
     * classes.
     *
     */
    class PowerSaver : public TouchResponder
    {
    protected:
        
        enum StepDirection
        {
            DIM_NONE,
            DIM_UP,
            DIM_DOWN
        };
        
        Timer dimTimer, sleepTimer;
        int dimBright;
        int fullBright;
        bool enabled;
        StepDirection stepDir;
        
        void dimStep();
        void sleepStep();
        void undimStep();
        
    public:

        /**
         * @brief Construct a auto dimmer and sleep timer.
         *
         * @param dimTimeoutMs The delay before display is dimmed (in milliseconds)
         * @param sleepTimeoutMs The delay before sleep mode is entered (in milliseconds)
         * @param dimBrightness The brightness level when display is dimmed (0 to 255)
         * @param fullBrightness The full brightness level (normal mode) (0 to 255)
         */
        PowerSaver(int dimTimeoutMs = 10000, int sleepTimeoutMs = 10000, int dimBrightness = 30, int fullBrightness = 255);

        PowerSaver(PowerSaver &other);

        PowerSaver& operator=(PowerSaver &other);

        /**
         * @brief Starts the timer that will dim the display after the chosen timeout.
         * 
         * This call stops any running sleep timer. After the dim timer fires,
         * the sleep timer is automatically started.
         */
        void startDimTimer();

        /**
         * @brief Starts the sleep timer, that will sleep Mono after the chson timeout.
         * 
         * This will stop a running *dim timer*, and trigger sleep mode on timeout.
         */
        void startSleepTimer();

        /**
         * @brief Immidiately dim the display and then start the sleep timer.
         * 
         * This will (asynchronously) dim the display to the chosen brightness level.
         * WHen the display has been dimmed, the sleep timer is started.
         */
        void dim();

        /**
         * @brief Immidiately undim the display to full brightness.
         *
         * This will stop the sleep timer and re-start the dim timer.
         */
        void undim();

        /**
         * @brief Disable the PowerSaver all together
         * 
         * No dimming or sleep is triggered. You must call @ref startDimTimer
         * to re-enabled the PowerSaver.
         */
        void deactivate();
        
        virtual void respondTouchBegin(TouchEvent &event);
    };
}

#endif /* power_saver_h */
