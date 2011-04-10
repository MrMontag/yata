/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "SessionLoader.h"
#include "MainWindow.h"
#include "YApplication.h"
#include "session/ApplicationSession.h"
#include "session/FileSession.h"
#include "session/SessionIO.h"
#include <QDir>

void SessionLoader::readSession(MainWindow * win)
{
    SessionIO sessionIO;
    ApplicationSession appSession;

    std::string sessionFile = nativeSessionPath();
    sessionIO.readSession(&appSession, sessionFile);

    for(size_t i = 0; i < appSession.fileCount(); i++) {
        const FileSession & fileSession = appSession.fileAt(i);
        win->addFile(QString::fromStdString(fileSession.path));
    }

    win->setCurrentFileIndex(appSession.currentIndex());
}

void SessionLoader::writeSession(MainWindow * win)
{
    SessionIO sessionIO;
    ApplicationSession appSession;
    appSession.setCurrentIndex(win->currentFileIndex());

    std::vector<FileSession> sessions;
    win->fileSessions(&sessions);
    for(std::vector<FileSession>::const_iterator itr = sessions.begin();
        itr != sessions.end();
        ++itr) {
        appSession.addFile(*itr);
    }

    std::string sessionFile = nativeSessionPath();
    sessionIO.writeSession(appSession, sessionFile);
}

std::string SessionLoader::nativeSessionPath()
{
    QString sessionFile = YApplication::sessionFilePath();
    sessionFile = QDir::toNativeSeparators(sessionFile);
    return sessionFile.toStdString();
}
