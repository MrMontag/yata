# This file is part of yata -- Yet Another Tail Application
# Copyright 2010-2011 James Smith
# 
# Licensed under the GNU General Public License.  See license.txt for details.

TEMPLATE = app
TARGET = 
DEPENDPATH += . app document fileio gui resource search session view watcher

# TODO: make the following line unnecessary.
INCLUDEPATH += . gui session app document fileio view search watcher

# Input
HEADERS += app/YApplication.h \
           document/YFileCursor.h \
           document/YTextDocument.h \
           fileio/BufferedFile.h \
           fileio/FileBlockReader.h \
           gui/MainWindow.h \
           gui/SearchWidget.h \
           gui/YTabMenuButton.h \
           gui/YTabWidget.h \
           search/BaseSearch.h \
           search/DocumentSearch.h \
           search/FileSearch.h \
           search/SearchInfo.h \
           session/AppSession.h \
           session/FileSession.h \
           session/SearchSession.h \
           session/SessionCommon.h \
           session/SessionIO.h \
           session/SessionLoader.h \
           view/FullLayout.h \
           view/LayoutStrategy.h \
           view/PartialLayout.h \
           view/TailView.h \
           watcher/YFileSystemWatcher.h \
           watcher/YFileSystemWatcherThread.h
FORMS += gui/MainWindow.ui gui/SearchWidget.ui
SOURCES += main.cpp \
           app/YApplication.cpp \
           document/YFileCursor.cpp \
           document/YTextDocument.cpp \
           fileio/BufferedFile.cpp \
           fileio/FileBlockReader.cpp \
           gui/MainWindow.cpp \
           gui/SearchWidget.cpp \
           gui/YTabMenuButton.cpp \
           gui/YTabWidget.cpp \
           search/BaseSearch.cpp \
           search/DocumentSearch.cpp \
           search/FileSearch.cpp \
           search/SearchInfo.cpp \
           session/AppSession.cpp \
           session/FileSession.cpp \
           session/SearchSession.cpp \
           session/SessionIO.cpp \
           session/SessionLoader.cpp \
           view/FullLayout.cpp \
           view/LayoutStrategy.cpp \
           view/PartialLayout.cpp \
           view/TailView.cpp \
           watcher/YFileSystemWatcher.cpp \
           watcher/YFileSystemWatcherThread.cpp
RESOURCES += resource/TailViewResources.qrc

LIBS += -lyaml-cpp
