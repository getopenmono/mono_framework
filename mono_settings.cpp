// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "mono_settings.h"

#include <string.h>
#include <project.h>
#include <mbed_debug.h>

using namespace mono::io;

MonoSettings::MonoSettings()
{
    Settings = this;
}

int MonoSettings::size() const
{
    return 2^11;
}

int MonoSettings::read(SettingLocations location, SettingSizes size, void *destination)
{
    CyEEPROM_Start();

    void *memoryPosition = (void*)(CY_EEPROM_BASE+location);
    memcpy(destination, memoryPosition, size);

    CyEEPROM_Stop();

    return 0;
}

int MonoSettings::write(SettingLocations location, SettingSizes size, const void *value)
{
    CyEEPROM_Start();

    // align position to EEPROM row boundly (PSOC5 this is 16)
    uint32_t locationAligned = ((location+15) & (~0x0F));
    uint16_t rowAddr = locationAligned / 16;

    int state = CyWriteRowFull(EEPROMAddress, rowAddr, (const uint8_t*)value, CY_EEPROM_SIZEOF_ROW);
    debug("EEPROM write state: %0x%x\n\r",state);

    CyEEPROM_Stop();

    return state;
}
