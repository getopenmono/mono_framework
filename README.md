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

To build the framework you need several prerequisites. First you should checkout the following repos:

* [mbed](https://github.com/getopenmono/mbed) : Provide basic I/O and filesystem support (required by *mbedcomp*)
* [mono_psoc5_library](https://github.com/getopenmono/mono_psoc5_library) : Provide the Cypress Hardware layer (required by *mbedcomp*)
* [mbedcomp](https://github.com/getopenmono/mbedComp) : Provide the mbed compability for Mono's MCU (required by *Mono Framework*)

Checkout all repositories (including this one) in the same directory:

```
$ mkdir mono; cd mono
$ git clone https://github.com/getopenmono/mbed.git
$ git clone https://github.com/getopenmono/mono_psoc5_library.git
$ git clone https://github.com/getopenmono/mbedComp.git
$ git clone https://github.com/getopenmono/mono_framework.git
$ cd mono_framework
```

Now you should be in the *mono_framework* repository. We now must setup the the correct paths in the `Makefile`. Open the file `Makefile` in your favorite editor.

In line 2 you see the define for the path `ARCH`. This to the prefix for the compiler to be used. To must change this to your installation of the [ARM GCC Embedded](https://launchpad.net/gcc-arm-embedded) toolchain. If you have Mono SDK installed you can use the included compiler. Just change the path to:

* MacOS: `/usr/local/openmono/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-`
* Windows: `C:/openmono/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-`
* Linux (Ubuntu): `arm-none-eabi-`

Save the file, can compile the framework with:

```
$ make release
```

Now the libraries are being compiled. First the system builds `mono_psoc5_library`, then `mbedComp` and lastly the framework itself. The resulting library and header files is place in a directory called `dist`, next the repositories:

```
$ cd ../dist
$ ls -l mono
```

The compiled result are all header files and 4 static libraries:

* `mono_framework.a` : This framework 
* `mbedlib.a` : The mbed library
* `monoCyLib.a` : The Cypress hardware library
* `CyComponentLibrary.a` : Cypress dependencies

You can replace these files with the once in your Mono SDK installation folder: `openmono/mono`. Then you can take advantage of your own modifications to Mono framework.


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
