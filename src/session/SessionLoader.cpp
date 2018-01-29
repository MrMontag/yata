/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gui/MainWindow.h"
#include "search/SearchInfo.h"
#include "session/SessionLoader.h"
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
    win->setCurrentOpenDirectory(QString::fromStdString(appSession.lastOpenDirectory()));

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

    appSession.setLastOpenDirectory(win->currentOpenDirectory().toStdString());

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
