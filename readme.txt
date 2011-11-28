Yata -- Yet Another Tail Application

Building requirements:
- Qt 4.7 or later
- yaml-cpp 0.2.6 (see http://code.google.com/p/yaml-cpp for downloads and
  instructions)
- C++ compiler capable of building Qt (GNU gcc is recommended)

To build on Unix:

- Ensure yaml-cpp is built and installed in a standard place (i.e., /usr/lib or
  /usr/local/lib)
- Change to the root of the yata source tree and run the following commands:
    qmake
    make
    make install
  Note that you may need to be root to run the last command.

By default, yata will install to /usr/local/bin. To change the default, specify
the desired directory with INSTALLDIR as an argument to qmake. For example, to
install to /usr/bin, use:
    qmake INSTALLDIR=/usr/bin

To build on Windows:

The easiest way to build on Windows is to obtain the open source Qt SDK from
http://qt.nokia.com. (This will give you the Qt toolkit, plus MinGW, a version
of GNU gcc for windows. You will still need to download yaml-cpp from
code.google.com/p/yaml-cpp.) Here are the steps:

- Ensure yaml-cpp is built and installed. (You may need to download cmake for
  windows for this step.) Make a note of where the libraries and headers are
  installed.
- Open a command window, or open Qt Creator. (The rest of the directions
  assumes the command line.)
- Change to the root of the yata source tree and run qmake. Depending on where
  yaml-cpp is installed, you may need to specify its path. By default, qmake
  will look for yaml-cpp one directory up from where yata's source tree is. If
  you have installed it elsewhere, you should set the YAML-CPP variable on the
  command line. For instance, if yaml-cpp is installed in C:\yaml-cpp, you
  would run qmake like this:

  qmake YAML-CPP=C:/yaml-cpp

  Note the use of forward slashes for directory separators.
- Run mingw32-make.

Note that you may need to add qmake and mingw32-make to your %PATH% environment
variable in order for the commands to succeed.

To run:
yata [file[s]]

Please note that this software is alpha quality software,
and is not intended for general use yet.

Have fun!
