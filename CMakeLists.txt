# This file is part of yata -- Yet Another Tail Application
# Copyright 2010-2011 James Smith
# 
# Licensed under the GNU General Public License.  See license.txt for details.
project(yata)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.7.0 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

include_directories(${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Release)

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

set(Yata_Srcs
    BaseSearch.cpp
    BufferedFile.cpp
    DocumentSearch.cpp
    FileBlockReader.cpp
    FileSearch.cpp
    FullLayout.cpp
    LayoutStrategy.cpp
    main.cpp
    MainWindow.cpp
    PartialLayout.cpp
    SearchWidget.cpp
    TailView.cpp
    YApplication.cpp
    YFileCursor.cpp
    YFileSystemWatcher.cpp
    YFileSystemWatcherThread.cpp
    YTabWidget.cpp
    YTextDocument.cpp)

set(Yata_Rsrcs TailViewResources.qrc)

qt4_wrap_cpp(Yata_Moc_Srcs ${Yata_Mocs})
qt4_wrap_ui(Yata_Ui_Srcs ${Yata_Uis})
qt4_add_resources(Yata_Rsrcs_Srcs ${Yata_Rsrcs})

add_executable(yata
    ${Yata_Srcs}
    ${Yata_Moc_Srcs}
    ${Yata_Ui_Srcs}
    ${Yata_Rsrcs_Srcs})
target_link_libraries(yata ${QT_LIBRARIES})

