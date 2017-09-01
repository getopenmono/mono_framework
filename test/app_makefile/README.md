# Full build Makefile

**This folder contains a *Makefile* that builds mono apps directly from framework source.**

This is used by the app test system, to pull mono apps from a Git repo and then build them against the current framework source code.

* `Makefile`: Makefile that compiles all framework source file, along with the app sources.
* `main.cpp`: Normally mono apps does not contain a `main()`, this function is defined here.