// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef mono_settings_h
#define mono_settings_h

#include "settings_interface.h"

namespace mono { namespace io {

    /**
     * @brief Persistant Settings store based on Mono's EEPROM
     * 
     * **NOTE**: This setting implementation uses the PSoC5 EEPROM as settings
     * store. The EEPROM has limited write cycles, up to 1 million writes.
     *
     * Therefore do not take care when saving settings, write as rare as
     * possible!
     *
     * @see ISettings
     */
    class MonoSettings : public ISettings
    {
    private:

        static const int EEPROMAddress = 0x40;

    public:

        /**
         * @brief Fast lightweight contructor for the Mono Settings (EEPROM)
         * 
         * **NOTE**: This setting implementation uses the PSoC5 EEPROM as settings
         * store. The EEPROM has limited write cycles, up to 1 million writes.
         *
         * Therefore do not take care when saving settings, write as rare as
         * possible!
         */
        MonoSettings();

        int size() const;

        int write(SettingLocations location, SettingSizes size, void const *value);

        int read(SettingLocations location, SettingSizes size, void *destination);
    };

} }

#endif /* mono_settings_h */
