/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "MainWindow.h"
#include "SearchInfo.h"
#include "SessionLoader.h"
#include "YApplication.h"
#include "session/AppSession.h"
#include "session/FileSession.h"
#include "session/SearchSession.h"
#include "session/SessionIO.h"
#include <QDir>

void SessionLoader::readSession(MainWindow * win)
{
    SessionIO sessionIO;
    AppSession appSession;

    std::string sessionFile = nativeSessionPath();
    sessionIO.readSession(&appSession, sessionFile);

    SearchInfo::Container searchList;
    for(size_t i = 0; i < appSession.searchCount(); i++) {
        const SearchSession & searchSession = appSession.searchAt(i);
        searchList.push_back(SearchCriteria(
            QString::fromStdString(searchSession.expression),
            searchSession.isCaseSensitive,
            searchSession.isRegex));
    }

    SearchInfo::instance().populateSearchList(&searchList);

    for(size_t i = 0; i < appSession.fileCount(); i++) {
        const FileSession & fileSession = appSession.fileAt(i);
        win->addFile(QString::fromStdString(fileSession.path));
    }

    win->setCurrentFileIndex(appSession.currentIndex());

    QByteArray qGeometry;
    const AppSession::GContainer & geometry = appSession.geometry();
    for(AppSession::GContainer::const_iterator itr = geometry.begin();
        itr != geometry.end(); ++itr) {
        qGeometry.push_back(*itr);
    }

    win->restoreGeometry(qGeometry);
}

void SessionLoader::writeSession(MainWindow * win)
{
    SessionIO sessionIO;
    AppSession appSession;
    appSession.setCurrentIndex(win->currentFileIndex());

    const SearchInfo & searchInfo = SearchInfo::instance();

    for(SearchInfo::const_iterator sc = searchInfo.begin(); sc != searchInfo.end(); ++sc) {
        SearchSession searchSession(
            sc->expression.toStdString(), sc->isRegex, sc->isCaseSensitive);
        appSession.addSearch(searchSession);
    }


    std::vector<FileSession> sessions;
    win->fileSessions(&sessions);
    for(std::vector<FileSession>::const_iterator itr = sessions.begin();
        itr != sessions.end();
        ++itr) {
        appSession.addFile(*itr);
    }

    QByteArray qGeometry = win->saveGeometry();

    AppSession::GContainer geometry;

    for(int i = 0; i < qGeometry.size(); i++) {
        geometry.push_back(qGeometry[i]);
    }

    appSession.setGeometry(geometry);

    std::string sessionFile = nativeSessionPath();
    sessionIO.writeSession(appSession, sessionFile);
}

std::string SessionLoader::nativeSessionPath()
{
    QString sessionFile = YApplication::sessionFilePath();
    sessionFile = QDir::toNativeSeparators(sessionFile);
    return sessionFile.toStdString();
}
