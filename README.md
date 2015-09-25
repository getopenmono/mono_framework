# Mono Framework
This is the embedded Mono framework, for interacting with Wifi display etc.

We are currently developing the framework, so nothing is static. Interfaces and APIs are very likely to change.

## Application Structure
The framework basic application structure consists of 3 classes:

 * _The Application Controller_: A class that you create, that is the entry point for the application. We expect the _AppController_ to handle crucial system wide events, like power and standby operations.
 * _The main.cpp file_: The classic entry point for C++ applications. The `main` function must create the AppController and start the application run loop, just like Qt applications do.
 * _The Application Context_: This is a singleton class that is automatically instantiated in the global context. The _AppContext_ holds references to the hardware controller; like the display, power system and wireless module.

By using these 3 classes we can hide many initialization steps from your code. To draw graphics on the display, you just need to create a _View_ and that it! The _AppContext_ takes care of initializing the display system.

## Display System
The mono display system consist of 3 abstractions:

* _The Display Controller Interface_: A generic interface for communication with the display chip. This interface uses either SPI or parallel communication, and send commands to the display chip.
* _The Display Painter_: The painter class can send drawing commands to a display interface. It emits pixels from basic drawing commands like rect, lines and text glyphs.
* _The Views_: Like moderns UI frameworks, mono uses View object to render graphics on the display. All views share a common painter object, and draw themselves using the painters draw functions. Because mono is a small device, we do not have a traditional view hierarchy. This means a view cannot contains other views.

Our goal is to make drawing UIs on mono very convenient. We do not want you to paint your own UI with rectangles and lines, but using our Button and TextLabel views!


## Wireless communication stack
We have re-implemented and re-structured the communication stack for our wireless module. The stack provide the SPI communication with the wireless module. So far we still need a clear and good abstraction between module specific code and generic code like the network stack.

## Power System
Work has begun on making a easy way of getting mono into and out of sleep mode. With many peripherals and hardware I/O components, getting power consumption down during sleep is a challenge. The power system is a set on classes that handle the power supply IC and the internal MCU peripherals. See the files:

* *power_management_interface.h*: Interface for overall power control
* *power_subsystem_interface.h*: Interface for power supply IC control classes
* *power_aware_interface.h*: Interface for classes that must respond to power related events.

##Documentation ToDo's

* Getting Started Tutorial
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

Copyright(C) Monolit ApS 2015
