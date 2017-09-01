// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef file_system_h
#define file_system_h

#include <SDFileSystem.h>
#include <power_aware_interface.h>

namespace mono { namespace io {

    /**
     * @brief Mono specific version of mbed's SDFileSystem class
     *
     * Access the filesystem on an SD Card using standard C lib functions:
     *
     * * fopen
     * * fread
     * * fwrite
     * * fclose
     *
     * You should include *stdio.h* and use I/O functions as in normal C on a
     * OS.
     *
     * ### Reset and wake
     *
     * You should atke care not to write to the SD card immediately after a
     * reset or wake up.SD cards might need to time from power-on to they react
     * on SD initialization sequence. Therefore you should at least wait some
     * milliseconds to allow the SD to be powered, before you start to write
     * data.
     *
     * ### Example
     *
     * You should this class as a member of your global *AppController* class,
     * such that it is first initialized when your *AppController* is
     * contructed.
     *
     * Add as a variable on *AppController*:
     *
     * @code
     * class AppController : public mono::IApplication
     * {
     *  public:
     *    mono::io::FileSystem fs;
     * }
     * @endcode
     *
     * Then call the designated initializor oin your *AppController*'s
     * constructor:
     *
     * @code
     * AppController::AppController() :
     *   fs("sd")
     * {
     *    // ...
     * }
     * @endcode
     *
     * Now the SD card is mounted at: /sd
     *
     * This class adds support for sleep mode to *mbed* FS functionality.
     */
    class FileSystem : mono::power::IPowerAware
#ifndef EMUNO
    , SDFileSystem
#endif
    {
    public:

        const char *mntPnt;

        /** Construct an invalid file system driver */
        FileSystem();

        /**
         * @brief Initialize the File system for a attached SD Card.
         */
        FileSystem(const char *mountPoint = "/");

        // MARK: Power Aware Interface

        void onSystemEnterSleep();
        void onSystemWakeFromSleep();

        ~FileSystem();
    };

} }

#endif /* file_system_h */
