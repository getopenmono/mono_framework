# Mono Framework
This is the embedded Mono framework, for interacting with Wifi display etc.

We are currently developing the framework, so nothing is static. Interfaces and APIs are very likely to change.

## Wireless communication stack
We have re-implemented and re-structured the communication stack for our wireless module. The stack provide the SPI communication with the wireless module. So far we still need a clear and good abstraction between module specific code and generic code like the network stack.


The software is released under the open source MIT License.

Copyright(C) Monolit ApS 2015
