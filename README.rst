Doubleater
*****

Introduction
============

Doubleater is a project oriented to improve privacity of Microsoft Windows operating system, blocking most of all telemetry, real time scanning files and diagnostic connections.
It is mainly developed for the windows edition: Windows 10 Enterprise LTSC (21H2 version), but it should work perfectly on any Windows version higher than Windows 10, as well as Windows 7/8.1 with some code modifications. 
It is highly recommended to have knowledge of Windows Internal and programming in C/C++, if so.

License
=======

Doubleater is distributed under the Creative Commons Attribution 4.0 International License. 
See `LICENSE`_ for details.

.. _`LICENSE`: LICENSE

Building Doubleater
==============

Supported Windows Versions
-------------------

* Microsoft Windows 7 (Service Pack 1 or later)
* Microsoft Windows 8.1
* Microsoft Windows 10 (Recommended)
* Microsoft Windows 11 (Recommended)

Trying to compile this project in a UNIX-based operating system will result on a waste of time.

Building Debloater from Scratch
---------------------------

There are two ways for building Debloater under Windows:

1. Compile with MSVC from VS 2019 or later.
   You can open Doubleater.sln file to load the whole project to build it from Visual Studio

2. Compile with CMake.
     $ mkdir build && cd build
     $ cmake ..
     $ cmake --build . --config Release


In both cases, check `build/Release` to find the built (.exe) file.