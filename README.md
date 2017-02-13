# SJOne LPC1758 Free RTOS

## Overview

Reason for fork:

* Development environment for SJOne board was heavily focused on Windows
    * Didn't really want to rely on a Windows VM so a method to be "platform agnostic" was explored. Build environment was ported to Mac with Eclipse (to be uploaded when ready)
    * To be even more compatible with other build systems a Makefile was introduced with just the source (which is the main purpose of this repository: to keep an eye on that Makefile).

## TODO

- [ ] Add instructions on editing Makefile 
- [ ] Add instructions on setting up the tools required to use this Makefile
- [ ] Add "development" instructions

## Original 
Original repo: https://gitlab.com/preet/lpc1758_freertos
```
-----------------------------------------------------------------------------
To get started, have a look at L5_Application/main.cpp and the example tasks.

You can change a #define 0 to #define 1 for one example at a time and look
through the code of each example.

The general idea is to interact with the project through terminal commands.
-----------------------------------------------------------------------------
See "ChangeLog" for the changes made to the sample project source code.
-----------------------------------------------------------------------------
```