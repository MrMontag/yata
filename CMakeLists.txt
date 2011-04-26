
# This file is part of yata -- Yet Another Tail Application
# Copyright 2010-2011 James Smith
# 
# Licensed under the GNU General Public License.  See license.txt for details.
project(yata)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.7.0 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_INCLUDE_PATH})

include_directories(${CMAKE_BINARY_DIR})

if(DEFINED CONFIG)
    message("-- Setting CMAKE_BUILD_TYPE to ${CONFIG}")
    set(CMAKE_BUILD_TYPE ${CONFIG})
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

# cmake apparently doesn't distinguish between g++ and winres, and so winres gets these options, and
# chokes. So, for now, disable extra warnings if on mingw.
if(NOT MINGW)
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
endif(NOT MINGW)

set(Yata_Mocs
    MainWindow.h
    SearchWidget.h
    TailView.h
    YFileSystemWatcher.h
    YFileSystemWatcherThread.h
    YTabWidget.h)

set(Yata_Uis
    MainWindow.ui
    SearchWidget.ui)

set(Yata_Session_Srcs
    session/AppSession.cpp
    session/FileSession.cpp
    session/SearchSession.cpp
    session/SessionIO.cpp)

set(Yata_Srcs
    BaseSearch.cpp
    BufferedFile.cpp
    DocumentSearch.cpp
    FileBlockReader.cpp
    FileSearch.cpp
    FullLayout.cpp
    LayoutStrategy.cpp
    MainWindow.cpp
    PartialLayout.cpp
    SearchWidget.cpp
    SessionLoader.cpp
    TailView.cpp
    YApplication.cpp
    YFileCursor.cpp
    YFileSystemWatcher.cpp
    YFileSystemWatcherThread.cpp
    YTabWidget.cpp
    YTabMenuButton.cpp
    YTextDocument.cpp
    main.cpp)

set(Yata_Rsrcs TailViewResources.qrc)

set(Win32_Rsrcs yata_win.rc)

qt4_wrap_cpp(Yata_Moc_Srcs ${Yata_Mocs})
qt4_wrap_ui(Yata_Ui_Srcs ${Yata_Uis})
qt4_add_resources(Yata_Rsrcs_Srcs ${Yata_Rsrcs})

add_executable(yata
    ${Yata_Srcs}
    ${Yata_Session_Srcs}
    ${Yata_Moc_Srcs}
    ${Yata_Ui_Srcs}
    ${Yata_Rsrcs_Srcs}
    ${Win32_Rsrcs})
if(MINGW)
    # Disable the console window
    target_link_libraries(yata "-mwindows")
endif(MINGW)
target_link_libraries(yata ${QT_LIBRARIES} ${YAML_LIB})

