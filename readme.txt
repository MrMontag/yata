Yata -- Yet Another Tail Application

Building requirements:
- Qt 4.6 or later
- cmake 2.6 or later (available at http://www.cmake.org; and is usually packaged
  with most Linux distributions)
- yaml-cpp 0.2.6
- C++ compiler capable of building Qt (GNU gcc is recommended)

On Windows, the easiest way to build is to download the Qt SDK
from http://qt.nokia.com/downloads.

To build on Linux:
Make a directory where the build object files, generated code, etc. will go.
This can be anywhere, however I usually put it under the source directory.
So, assuming your current working directory is the source directory:

mkdir build && cd build

Next, run cmake with the directory containing the CMakeLists.txt file. So, if
you made a build directory in the source directory:

cmake ..

Then, run make:

make

To build on Windows:

[TODO]

To run:
yata [file[s]]

Please note that this software is alpha quality software,
and is not intended for general use yet.

Have fun!
