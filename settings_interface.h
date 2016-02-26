// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef settings_interface_h
#define settings_interface_h

#include "date_time.h"
#include "touch_system_interface.h"

namespace mono { namespace io {

    /**
     * @brief This abstract interface defines the memory locations in the settings store
     * 
     * Mono can store application or framework settings in a persistant memory 
     * store. This interface provide the memory layout and default accessors to
     * that persistant memory.
     *
     * The Settings Interface models a static memory space to preserve data values
     * across reboots and power cycles.
     *
     * The mono framework uses some pre-defined locations in the (persistant) 
     * memory space, to store configurations such as touch calibration, the current
     * date and time and the wifi setup.
     *
     * You can also save custom configuration data in this memory store, as long
     * as you do it below the position defined by the enum value:
     * `USER_APP_SETTINGS_LOCATION`
     *
     * Since the persistant memory store is hardware dependant to should use the
     * the implementation class called @ref Settings to interact with the 
     * setting store.
     *
     * @see Settings
     */
    class ISettings
    {
    public:

        enum SettingSizes
        {
            DATETIME_SIZE = sizeof(DateTime),
            CALIBRATION_SIZE = sizeof(TouchCalibration),
            WIFI_CONFIGURATION_SIZE = 64 // we just set it ot 64 bytes for now
        };

        enum SettingLocations
        {
            DATETIME_LOCATION = 0x0,
            CALIBRATION_LOCATION = DATETIME_LOCATION+DATETIME_SIZE,
            WIFI_CONFIGURATION_LOCATION = CALIBRATION_LOCATION+CALIBRATION_SIZE,
            USER_APP_SETTINGS_LOCATION = WIFI_CONFIGURATION_LOCATION+WIFI_CONFIGURATION_SIZE
        };

        /**
         * @brief Return the size of the persistant memory store in bytes
         *
         * @return the byte size of the memory store
         */
        virtual int size() const = 0;

        /**
         * @brief Write a setting to the persistent settings store
         * 
         */
        virtual int write(SettingLocations location, SettingSizes size, void const *value) = 0;

        /**
         * @brief Retrieve a setting from the persistant settings store
         * 
         */
        virtual int read(SettingLocations location, SettingSizes size, void *destination) = 0;

    };

    /**
     * A pointer to the
     * 
     */
    static ISettings* Settings = 0;
} }

#endif /* settings_interface_h */
