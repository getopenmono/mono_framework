// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef displaySimTest_application_context_h
#define displaySimTest_application_context_h

#include <display_controller_interface.h>
#include <application_run_loop.h>
#include "power_management_interface.h"
#include "touch_system_interface.h"
#include "queue_interrupt.h"
#include "temperature_interface.h"
#include "accelerometer_interface.h"
#include "buzzer_interface.h"
#include "rtc_interface.h"

namespace mono {
    
    class IApplication; // forward declaretion of the IApplication interface
    
    /**
     * The Application context class is a singleton class that is automatically 
     * instanciated by the framework. You should not need to interact with it
     * directly. It is allocated on the stack, with its member objects.
     *
     * The application context controls the application event loop at hardware
     * event inputs. It is essential for communicating with Serial-USB and the
     * display.
     *
     * Depending on the execution context (hardware mono device or simulator),
     * different subclasses of this interface it used. This interface is provided
     * to give your application code a pointer to the concrete implementation
     * of the application context. Regardless of running on a simulator or the 
     * actual device.
     *
     */
    class IApplicationContext
    {
    protected:
        
        // MARK: Abstract Protected Methods
        
        /**
         * Subclasses should overirde this method to make the sysetm goto sleep
         */
        virtual void enterSleepMode() = 0;
        
        /**
         * Subclasses should override this to enable sleep mode for a specefic
         * interval only.
         */
        virtual void sleepForMs(uint32_t ms) = 0;
        
        /**
         * Subclasses must implement this to enable the "Reset On User Button"
         * behaviour. See @ref ResetOnUserButton
         */
        virtual void resetOnUserButton() = 0;
        
        /**
         * Subclasses must implement this method to enable software resets.
         * See @ref SoftwareReset
         */
        virtual void _softwareReset() = 0;
        
        
        /**
         * Subclasses must implement this to enable software reset to application
         * See @ref SoftwareResetToApplication
         */
        virtual void _softwareResetToApplication() = 0;
        
        /**
         * Subclasses must implement this method to allow *reset to bootloader*
         * See @ref SoftwareResetToBootloader
         */
        virtual void _softwareResetToBootloader() = 0;
        
        // MARK: Protected Constructor
        
        /**
         * Protected constructor that must be called by the sub class. It sets up
         * needed pointers for the required subsystems. This ensure the pointers
         * are available when class members' constructors are executed.
         *
         * If this contructor did not setup the pointers, the PowerManagement
         * constructor would see the @ref Instance global equal `null`.
         */
        IApplicationContext(power::IPowerManagement *pwr,
                            AppRunLoop *runLp,
                            display::IDisplayController *dispCtrl,
                            ITouchSystem *tchSys,
                            QueueInterrupt *userBtn,
                            sensor::ITemperature *temp = 0,
                            sensor::IAccelerometer *accel = 0,
                            sensor::IBuzzer *buzzer = 0,
                            IRTCSystem *irtc = 0
                            ) :
            PowerManager(pwr),
            RunLoop(runLp),
            DisplayController(dispCtrl),
            TouchSystem(tchSys),
            UserButton(userBtn),
            Temperature(temp),
            Accelerometer(accel),
            Buzzer(buzzer),
            RTC(irtc)
        {
            IApplicationContext::Instance = this;
        }
        
    public:
        
        // MARK: Public Properties
        
        /**
         * Pointer to the global power management object, that controls power
         * related events and functions. Use this pointer to go into sleep mode'
         * or get the current battery voltage level.
         *
         * @brief A pointer the power management system
         */
        power::IPowerManagement *PowerManager;
        
        /**
         * A reference to the main run loop of the application.
         * This pointer must be instanciated be subclasses
         */
        AppRunLoop *RunLoop;
        
        /**
         * Pointer to the display interface controller object. The object itself
         * should be initialized differntly depending on the ApplicationContext
         *
         */
        display::IDisplayController *DisplayController;
        
        /**
         * @brief Pointer to the touch system controller object.
         * 
         * The touch system handles touch input from the display or other input
         * device. It must be initialized by an ApplicationContext implementation.
         *
         * The touch system is the source of @ref TouchEvent and delegate these
         * to the @ref TouchResponder classes.
         * It is the @ref ITouchSystem holds the current touch calibration. To
         * re-calibrate the touch system, you can use this reference.
         *
         * @see ITouchSystem
         */
        ITouchSystem *TouchSystem;
        
        
        /**
         * @brief The User Button queued interrupt handler.
         *
         * Here you add your application handler function for mono user button. 
         * To handle button presses you can set a callback function for the button
         * push.
         *
         * The callback function is handled in the @ref AppRunLoop, see the
         * @ref QueueInterrupt documentation for more information.
         * 
         * **Note** that the default initialized callback handler will toggle
         * sleep mode. This means that if you do not set your own handler, the
         * user button will put mono into sleep mode.
         * The default callback is set on the `.fall(...)` handler.
         *
         * Example for replacing the user button handler, with a reset handler:
         *
         * @code
         *
         *  // the button callback function
         *  void MyApp::handlerMethod()
         *  {
         *      IApplicationContext::SoftwareReset();
         *  }
         *
         *  // on reset install our own button handler callback
         *  void MyApp::monoWakeFromReset()
         *  {
         *      IApplicationContext::Instance->UserButton->fall<MyApp>(this, &MyApp::handlerMethod);
         *  }
         *
         * @endcode
         */
        QueueInterrupt *UserButton;
        
        /**
         * @brief A pointer to the Temperatrure sensor, if present in hardware
         * 
         * This is an automatically initialized pointer to the temperature
         * object, that is automatically created by the framework.
         */
        sensor::ITemperature *Temperature;
        
        /**
         * @brief A pointer to the Accelerometer, if present in hardware
         *
         * This is an automatically initialized pointer to the accelerometer
         * object, that is automatically created by the framework.
         */
        sensor::IAccelerometer *Accelerometer;

        /**
         * @brief A pointer to the buzzer, if present in hardware
         *
         * This is an automatically initialized pointer to the buzzer
         * object, that is automatically created by the framework.
         */
        sensor::IBuzzer *Buzzer;


        /**
         * @brief A Pointer to the current RTC interface, if such exists
         *
         * Mono has a RTC clock, that can control a system date time clock, that
         * is accessed by the @ref DateTime class
         *
         * You can start or stop the RTC using this interface. Note that this 
         * pointer might be NULL
         */
        IRTCSystem *RTC;

        // MARK: Public Methods
        
        /**
         * This method starts the global run/event loop for the mono application.
         * The method never returns, so a call to this function should be the
         * last line in your `main()` function.
         *
         * The event loop automatically schedules the sub system, such as the
         * network, inputs and the display.
         *
         * @brief Start the application run loop
         */
        virtual int exec() = 0;
        
        /** Sets a pointer to the mono application object */
        virtual void setMonoApplication(mono::IApplication *app) = 0;
        
//        /**
//         * The mono application controller should call this to give the
//         * Application Context a reference to itself.
//         *
//         * This will ensure the Application Controllers methods gets called.
//         */
//        static void SetMonoApplication(mono::IApplication &app)
//        {
//            IApplicationContext::Instance->setMonoApplication(&app);
//        }

        // MARK: Static Public Methods
        
        /**
         * Call this method to make mono goto sleep.
         * 
         * In sleep mode the CPU does not excute instruction and powers down into
         * a low power state.
         * The power system will turn off dynamically powered peripherals.
         *
         * *NOTE*: Before you call this method make sure that you configured a way
         * to go out of sleep.
         */
        static void EnterSleepMode()
        {
            IApplicationContext::Instance->enterSleepMode();
        }
        
        /**
         * @brief Enable *Reset On User Button* mode, where user button resets mono.
         * 
         * If your application encounters unmet dependencies (missing SD Card) or
         * gracefully handles any runtime errors, you can call this method.
         * When called, the run loop will reset mono if the user button (USER_SW)
         * is activated.
         *
         * This method allows you to reset mono using the user button, instead of
         * the reset button.
         */
        static void ResetOnUserButton()
        {
            IApplicationContext::Instance->resetOnUserButton();
        }
        
        /**
         * Enter MCU sleep mode for a short time only. Sets a wake-up timer us the
         * preferred interval, and calls the @ref EnterSleepMode method.
         * @param ms The number of milli-second to sleep
         */
        static void SleepForMs(uint32_t ms)
        {
            IApplicationContext::Instance->sleepForMs(ms);
        }
        
        /**
         * @brief Trigger a software reset of Mono's MCU
         * 
         * Calls the MCU's reset exception, which will reset the system. When reset
         * the bootloader will run again, before entering the application.
         */
        static void SoftwareReset() { IApplicationContext::Instance->_softwareReset(); }
        
        /**
         * @brief Trigger a software reset of MOno's MCU, that does not load 
         * the bootloader.
         * 
         * Use this to do a fast reset of the MCU.
         */
        static void SoftwareResetToApplication() { IApplicationContext::Instance->_softwareResetToApplication(); }
        
        /**
         * @brief Trigger a software reset, and stay in bootloader.
         * 
         * Calls the MCU reset exception, which resets the system. This method
         * sets bootloader parameters to stay in bootloader mode.
         *
         * **CAUTION: To get out of bootloader mode you must do a hard reset
         * (by the reset button) or program mono using monoprog.**
         */
        static void SoftwareResetToBootloader() { IApplicationContext::Instance->_softwareResetToBootloader(); }
        
        // MARK: Static Public Properties
        
        /** Get a pointer to the global application context */
        static IApplicationContext* Instance;
        
    };
}


#endif
