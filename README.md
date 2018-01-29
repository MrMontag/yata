# Yata -- Yet Another Tail Application

Yata is a simple platform independent tail application, which is based on
[Qt](https://www.qt.io/) and [yaml-cpp](https://github.com/jbeder/yaml-cpp).

Currently Yata requires Qt 5.10.0 (or greater) and yaml-cpp 0.5.x.

## Contribute

:tada::tada: Thank you very much for any contribution to Yata!

You will find all information needed to get started within the
[README.md](README.md) (this file) and the [CONTRIBUTING.md](CONTRIBUTING.md).
Additionally this project adheres to the Contributor Covenant [Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code. Please report
unacceptable behavior to the e-mail address listed in [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Build

### Build on Unix (t.b.d. - outdated)

- Ensure yaml-cpp is built and installed in a standard place (i.e., /usr/lib or
  /usr/local/lib). Instructions for building yaml-cpp are in its source package.
- Ensure that development libraries for Qt are installed. Qt4 is available in
  the repositories of most major Linux distributions. For example, on Ubuntu,
  you would run the following command on a terminal:
    sudo apt-get install libqt4-dev
- Change to the root of the yata source tree and run the following commands:
    qmake
    make
    make install
  Note that you may need to be root to run the last command.

By default, yata will install to /usr/local/bin. To change the default, specify
the desired directory with INSTALLDIR as an argument to qmake. For example, to
install to /usr/bin, use:
    qmake INSTALLDIR=/usr/bin

### Build on Windows (t.b.d. - outdated)

The easiest way to build on Windows is to obtain the open source Qt SDK from
http://qt.nokia.com. (This will give you the Qt toolkit, plus MinGW, a version
of GNU gcc for windows. You will still need to download yaml-cpp from
code.google.com/p/yaml-cpp.) Here are the steps:

- Ensure yaml-cpp is built and installed. (You will need to download cmake for
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
  Note that you should use forward slashes ("/") for directory separators, *not* 
  backslashes ("\").
- Run this command:
    mingw32-make release

Note that you may need to add the directories in which qmake and mingw32-make
are located to your %PATH% environment variable in order for the commands to
succeed.
