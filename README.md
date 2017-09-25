# Mono Framework

[![Build Status](https://travis-ci.org/getopenmono/mono_framework.svg?branch=master)](https://travis-ci.org/getopenmono/mono_framework) [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/getopenmono/mono_framework?branch=master&svg=true "AppVeyor Build Status")](https://ci.appveyor.com/project/stoffera/mono_framework) [![Docker Build Statu](https://img.shields.io/docker/build/monolit/framework.svg)](https://hub.docker.com/r/monolit/framework/) [![Documentation Status](https://readthedocs.org/projects/mono-developer-documentation/badge/?version=latest)](http://developer.openmono.com/en/latest/?badge=latest) [![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE.txt) [![Twitter Follow](https://img.shields.io/twitter/follow/getopenmono.svg?style=social&label=Follow)](https://twitter.com/getopenmono)

This is a framework for developing [Mono](http://openmono.com) applications.  For documentation on how to install and use this framework, see the [developer site](http://developer.openmono.com/).  If you want to generate the documentation, see [DOXYGEN.md](src/DOXYGEN.md).

This text describes how to build the framework.

## Setup

Install version 5 of the [ARM GCC Embedded toolchain](https://launchpad.net/gcc-arm-embedded).  Then add the path to your command-line environment if needed:

* MacOS: `/usr/local/openmono/gcc-arm-none-eabi-5_2-2015q4/bin`
* MacOS: XCode.  If problems, see [solution](https://bugreports.qt.io/browse/QTCREATORBUG-17494).
* Windows: `C:/openmono/gcc-arm-none-eabi-5_2-2015q4/bin`

## Building

Run

```
$ bash resources/setup_icons.sh resources/icons.mk.tmp 
```

(If you have problems with XCode, you might need to run `sudo xcode-select -s /Applications/Xcode.app/Contents/Developer`.)

Run
```
$ make release
```
to compile a new version of the framework into the `dist` folder.  The result is all header files and 4 static libraries:

* `mono_framework.a` : This framework.
* `mbedlib.a` : The mbed library.
* `monoCyLib.a` : The Cypress hardware library.
* `CyComponentLibrary.a` : Cypress dependencies.

To use these newly created files instead of the ones from the [official release](http://developer.openmono.com/en/latest/getting-started/install.html), overwrite the official file:

* MacOS: `sudo cp -r dist/mono/* /usr/local/openmono/mono/`
* Windows: Copy the `dist/mono` contents to `c:/OpenMono/mono`
* Linux: `sudo cp -r dist/mono/* /usr/lib/openmono/mono/`
