# Mono Framework
This is the Mono framework, for developing mono applications. It is designed to be simple and easy to use, and provide methods for the most common tasks.

## Overview
The framework consists of highlevel classes and interfaces to interact with hardware or to process data. The classes can be divided into these main categories:

 * **Display UI Widgets** (Text labels, Buttons, Progressbars or draw shapes)
 * **Touch Input** (Responders chain, raw touch events)
 * **Accelerometer** Interface API
 * **Thermometer** Interface API
 * **Power Management** Battery, sleep & wake notifications
 * **Wifi** Access Point setup
 * **HTTP Client** Fetch content from web servers
 * **Regular Expressions** Lightweight regex with capture groups
 * **File I/O** Posix like filesystem I/O (`fprintf`, `fread` & `fwrite`)
 * **Geometry Calculus** Classes to manipalte geometric shapes
 * **Interrupts & Timers** Receive callback notifications on H/W interrupts or timers
 * **USB Serial Console** Use *`printf` and `scanf` via the USB UART
 * **Image Decoding** Read images from SD Card

Are you looking for low level I/O like I<sup>2</sup>C, SPI, GPIO etc? We use [mbed](https://developer.mbed.org/handbook/Homepage#using-mbed-libraries) as the foundation, and this framework also gives you access to mbed. The mbed APIs are included in the mono framework.

## Building

We now must setup the the correct paths in the `Makefile`. Open the file `Makefile` in your favorite editor.

In line 2 you see the define for the path `ARCH`. This to the prefix for the compiler to be used. To must change this to your installation of the [ARM GCC Embedded](https://launchpad.net/gcc-arm-embedded) toolchain. If you have Mono SDK installed you can use the included compiler. Just change the path to:

* MacOS: `/usr/local/openmono/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-`
* Windows: `C:/openmono/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-`
* Linux (Ubuntu): `arm-none-eabi-`

Save the file, can compile the framework with:

```
$ make release
```

Now the libraries are being compiled. First the system builds `mono_psoc5_library`, then `mbedComp` and lastly the framework itself. The resulting library and header files are place in a directory called `dist`, next to the repositories:

```
$ ls -l ../dist/mono
```

The result is all header files and 4 static libraries:

* `mono_framework.a` : This framework
* `mbedlib.a` : The mbed library
* `monoCyLib.a` : The Cypress hardware library
* `CyComponentLibrary.a` : Cypress dependencies

To use these newly created files instead of the ones from the official release, overwrite the official file:

```
$ sudo cp -r ../dist/mono/* /usr/local/openmono/
```


replace the files with the ones in your Mono SDK installation folder: `openmono/mono`. Then you can take advantage of your own modifications to Mono framework.


##Documentation ToDo's

* Mono Framework: Architectural Overview
* * Mono Layer
* * mbed layer
* * Cypress / MCU layer
* Mono for Arduino Developers
* Coding for bare metal!
* In-Depth: Mono Framework Startup / Boot procedures
* In-Depth: Callbacks and interrupts
* In-Depth: Display System and Touch input
* In-Depth: Wifi and networking
* In-Depth: Power, battery and sleep
* * Interrupts during sleep

[Remember this article](https://msdn.microsoft.com/en-us/magazine/gg309172.aspx)

The software is released under the open source MIT License.

Copyright(C) Monolit ApS 2016
