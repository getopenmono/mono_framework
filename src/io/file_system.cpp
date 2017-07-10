// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "file_system.h"
#include <application_context.h>

using namespace mono::io;

FileSystem::FileSystem()
#ifndef EMUNO
    : SDFileSystem(NC, NC, NC, NC, "/")
#endif
{

}


FileSystem::FileSystem(const char *mountPoint)
#ifndef EMUNO
    : SDFileSystem(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_CLK, SD_SPI_CS, mountPoint)
#endif
{
    mntPnt = mountPoint;
    if (mono::IApplicationContext::Instance == 0)
        debug("You should not initialize FileSystem in global space! FS might not work!\r\n");
    else
        mono::IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);
}

void FileSystem::onSystemEnterSleep()
{
}

void FileSystem::onSystemWakeFromSleep()
{
#ifndef EMUNO
    _is_initialized = 0;
#endif
}

FileSystem::~FileSystem()
{
    if (mono::IApplicationContext::Instance != 0)
        mono::IApplicationContext::Instance->PowerManager->RemoveFromPowerAwareQueue(this);
}
