# This file is part of yata -- Yet Another Tail Application
# Copyright 2010-2011 James Smith
# 
# Licensed under the GNU General Public License.  See license.txt for details.

TEMPLATE = app
CONFIG += debug_and_release warn_on
CONFIG(debug, debug|release) {
    TARGET = yatad
} else {
    TARGET = yata
}

DEPENDPATH += . app document fileio gui resource search session view watcher

# TODO: make the following line unnecessary.
INCLUDEPATH += . gui session app document fileio view search watcher

# Input
HEADERS += \
    app/YApplication.h \
    app/YObjectPointer.h \
    dbg/DebugWindow.h \
    document/YFileCursor.h \
    document/YTextDocument.h \
    fileio/BufferedFile.h \
    fileio/FileBlockReader.h \
    gui/MainWindow.h \
    gui/PreferencesDialog.h \
    gui/SearchWidget.h \
    gui/YStatusBar.h \
    gui/YTabMenuButton.h \
    gui/YTabWidget.h \
    preferences/Preferences.h \
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

FORMS += \
    dbg/DebugWindow.ui \
    gui/MainWindow.ui \
    gui/PreferencesDialog.ui \
    gui/SearchWidget.ui

SOURCES += \
    app/main.cpp \
    app/YApplication.cpp \
    dbg/DebugWindow.cpp \
    document/YFileCursor.cpp \
    document/YTextDocument.cpp \
    fileio/BufferedFile.cpp \
    fileio/FileBlockReader.cpp \
    gui/MainWindow.cpp \
    gui/PreferencesDialog.cpp \
    gui/SearchWidget.cpp \
    gui/YStatusBar.cpp \
    gui/YTabMenuButton.cpp \
    gui/YTabWidget.cpp \
    preferences/Preferences.cpp \
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

RESOURCES += \
    resource/TailViewResources.qrc

win32 {
    isEmpty(YAMLCPP): YAMLCPP = $$PWD/../yaml-cpp
    INCLUDEPATH += $$YAMLCPP/include
    LIBS += -L$$YAMLCPP/lib
    RC_FILE += win/yata.rc
}

unix {
    isEmpty(INSTALLDIR): INSTALLDIR = /usr/local/bin
    target.path = $$INSTALLDIR
    INSTALLS += target
}

LIBS += -lyaml-cpp
