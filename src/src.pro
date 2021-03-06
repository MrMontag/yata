# Yata -- Yet Another Tail Application
#
# Copyright (C) 2010-2012 James Smith
# Copyright (C) 2018  Alexander Fust
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include(../res/appversion)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += 'APPVERSION=\\"$$VERSION\\"'
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app
CONFIG += c++14 debug_and_release warn_on

QMAKE_CXXFLAGS += -pedantic
build_pass:CONFIG(release, debug|release) {
    TARGET = yata
} else {
    TARGET = yatad
}

SOURCES += \
    main.cpp \
    YApplication.cpp \
    dbg/DebugWindow.cpp \
    document/YFileCursor.cpp \
    document/YTextDocument.cpp \
    fileio/BufferedFile.cpp \
    fileio/FileBlockReader.cpp \
    gui/ColorButton.cpp \
    gui/MainWindow.cpp \
    gui/PreferencesDialog.cpp \
    gui/SearchWidget.cpp \
    gui/TextColorWidget.cpp \
    gui/YStatusBar.cpp \
    gui/YTabMenuButton.cpp \
    gui/YTabWidget.cpp \
    preferences/Preferences.cpp \
    preferences/TextColor.cpp \
    search/BaseSearch.cpp \
    search/DocumentSearch.cpp \
    search/FileSearch.cpp \
    search/SearchInfo.cpp \
    session/AppSession.cpp \
    session/FileSession.cpp \
    session/SearchSession.cpp \
    session/SessionIO.cpp \
    session/SessionLoader.cpp \
    view/PartialLayout.cpp \
    view/TailView.cpp \
    watcher/YFileSystemWatcher.cpp \
    watcher/YFileSystemWatcherThread.cpp \
    view/ExactScrollBarController.cpp \
    view/ApproximateScrollBarController.cpp \
    view/ScrollBarStrategy.cpp \
    view/ScreenPosition.cpp

HEADERS += \
    YApplication.h \
    YObjectPointer.h \
    dbg/DebugWindow.h \
    document/BlockDataVector.h \
    document/BlockDataVector.inl \
    document/YFileCursor.h \
    document/YTextDocument.h \
    fileio/BufferedFile.h \
    fileio/FileBlockReader.h \
    gui/ColorButton.h \
    gui/MainWindow.h \
    gui/PreferencesDialog.h \
    gui/SearchWidget.h \
    gui/TextColorWidget.h \
    gui/YStatusBar.h \
    gui/YTabMenuButton.h \
    gui/YTabWidget.h \
    preferences/Preferences.h \
    preferences/TextColor.h \
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
    view/PartialLayout.h \
    view/TailView.h \
    watcher/YFileSystemWatcher.h \
    watcher/YFileSystemWatcherThread.h \
    view/ScrollBarStrategy.h \
    view/ExactScrollBarController.h \
    view/ApproximateScrollBarController.h \
    view/ScreenPosition.h \
    preferences/YAMLConverters.h

FORMS += \
    dbg/DebugWindow.ui \
    gui/MainWindow.ui \
    gui/PreferencesDialog.ui \
    gui/SearchWidget.ui \
    gui/TextColorWidget.ui

RESOURCES += \
    ../res/resources.qrc

win32 {
    isEmpty(YAMLCPP): YAMLCPP = $$PWD/../yaml-cpp
    INCLUDEPATH += $$YAMLCPP/include
    LIBS += -L$$YAMLCPP/lib
    RC_FILE += win/yata.rc
    build_pass:CONFIG(release, debug|release) {
        target.path = $$OUT_PWD
        target.commands = makensis /DEXESRCDIR=$$OUT_PWD\\release /DOUTDIR=$$OUT_PWD \
            /DVERSION=$$VERSION $$PWD\\win\\installer.nsi
        INSTALLS += target
    }
	LIBS += -lyaml-cpp
}

defineReplace(findFile) {
	baseDirs=$$1
	components=$$2
	for(baseDir, baseDirs) {
		for(component, components) {
			path=$$baseDir/$$component
			exists($$path) {
				return($$path)
			}
		}
	}
}

unix {
    INCLUDEPATH += ../yaml-cpp/include
    LIBS += -L$$PWD/../yaml-cpp/build -lyaml-cpp
}

#unix {
#	isEmpty(YAMLCPPINC) {
#		includeDirs=/usr/local/include /usr/include
#		prospectives=yaml-cpp03 yaml-cpp

#		YAMLCPPINC=$$findFile($$includeDirs, $$prospectives)

#		isEmpty(YAMLCPPINC): error(Could not find yaml-cpp include path)
#	}
#	message(YAMLCPPINC: $$YAMLCPPINC)
#	INCLUDEPATH += $$YAMLCPPINC
#    isEmpty(INSTALLDIR): INSTALLDIR = /usr/local/bin
#    target.path = $$INSTALLDIR
#    INSTALLS += target

#    LIBS += -l:libyaml-cpp.so.0.3
#}
