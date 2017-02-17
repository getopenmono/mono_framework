// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef buzzer_interface_h
#define buzzer_interface_h

#include <mbed.h>
#include <stdint.h>

namespace mono { namespace sensor {

    /**
     * @brief Generic Buzzer interface
     * 
     * This interface defines a generic API for buzzers used in the framework.
     * You should not construct any subclass of this interface yourself. The system
     * automatically creates a buzzer object for you, that you can obtain through
     * the @ref IApplicationContext:
     *
     * **Example**
     * @code
     * mono::sensor::IBuzzer *buzz = mono::IApplicationContext::Instance->Buzzer;
     * @endcode
     *
     * To make a short buzz sound do:
     * @code
     * mono::IApplicationContext::Instance->Buzzer->buzzAsync(100);
     * @endcode
     */
    class IBuzzer
    {
    protected:

        /**
         * @brief A Handler to call when buzzing finished
         * 
         * This member is setup when you provide a callback to the
         * @ref buzzAsync methods. Your subclass must call this when
         * buzzing ends.
         */
        mbed::FunctionPointer timeoutHandler;

    public:



        /**
         * @brief Buzz for a given period of time, then stop
         *
         * Sets the buzzer to emit a buzz for a defined number of milliseconds.
         * Then stop. This method is asynchronous, so it returns immediately. It
         * relies on interrupts to mute the buzzer later in time.
         *
         * You should not call it multiple times in a row, since it behaves 
         * asynchronously. Instread use @ref Timer to schedule multiple beeps.
         *
         * @param timeMs The time window where the buzzer buzzes, in milliseconds
         */
        virtual void buzzAsync(uint32_t timeMs) = 0;

        /**
         * @brief Stop any running buzz.
         * 
         * Use this method to cancel a buzz immediately. This method will not 
         * have any impact on callback functions. They will still be called, when
         * the buzz was suppose to end.
         */
        virtual void buzzKill() = 0;

        /**
         * @brief Buzz for some time, and when done call a C++ member function.
         * 
         * Sets the buzzer to emit a buzz for a defined number of milliseconds.
         * Then stop. This method is asynchronous, so it return immediately. It
         * relies on the run loop to mute the buzzer later in time. You also
         * provide a callback function, that gets called when the buzz is 
         * finished.
         *
         * You should not call it multiple times in a row, since it behaves
         * asynchronously. Instead you should use the callback function to make
         * a new beep.
         *
         * **Example**
         * @code
         * buzzAsync<AppController>(100, this, &AppController::buzzDone);
         * @endcode
         * This will buzz for 100 ms, then call `buzzDone`.
         *
         * @param timeMs The time window where the buzzer buzzes, in milliseconds
         * @param self The `this` pointer for the member function
         * @param member A pointer to the member function to call
         */
        template <typename Object>
        void buzzAsync(uint32_t timeMs, Object *self, void(Object::*member)(void))
        {
            buzzAsync(timeMs);
            timeoutHandler.attach<Object>(self, member);
        }

        /**
         * @brief Buzz for some time, and when done call a C function.
         *
         * Sets the buzzer to emit a buzz for a defined number of milliseconds.
         * Then stop. This method is asynchronous, so it return immediately. It
         * relies on the run loop to mute the buzzer later in time. You also
         * provide a callback function, that gets called when the buzz is
         * finished.
         *
         * You should not call it multiple times in a row, since it behaves
         * asynchronously. Instead you should use the callback function to make
         * a new beep.
         *
         * **Example**
         * @code
         * buzzAsync(100, &buzzDone);
         * @endcode
         * This will buzz for 100 ms, then call global C function `buzzDone`.
         *
         * @param timeMs The time window where the buzzer buzzes, in milliseconds
         * @param function A pointer to the global C function to call
         */
        void buzzAsync(uint32_t timeMs, void(*function)(void))
        {
            buzzAsync(timeMs);
            timeoutHandler.attach(function);
        }

    };

} }

#endif /* buzzer_interface_h */
