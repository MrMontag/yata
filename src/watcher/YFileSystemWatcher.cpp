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
#include "YFileSystemWatcher.h"

#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTimer>

#include <QtDebug>

YFileSystemWatcher::YFileSystemWatcher(const QString & filename)
    : m_watcher(new QFileSystemWatcher(this))
    , m_timer(new QTimer(this))
    , m_status(NoActivity)
    , m_filename(filename)
    , m_lastSize(0)
{
    connect(m_watcher.data(), SIGNAL(fileChanged(QString)), SLOT(on_watcher_fileChanged(const QString &)));
    connect(m_timer.data(), SIGNAL(timeout()), SLOT(on_timer_timeout()));

    m_watcher->addPath(m_filename);

    QFileInfo info(m_filename);
    m_lastModification = info.lastModified();
    m_lastSize = info.size();

    m_timer->start(250);
}

YFileSystemWatcher::~YFileSystemWatcher()
{
}

void YFileSystemWatcher::on_timer_timeout()
{
    QFileInfo info(m_filename);

    // On windows, it's possible to have a file with a creation
    // timestamp that is *after* its modification timestamp.
    // As a result, both things need to be checked.
    QDateTime lastRealModification = std::max(info.lastModified(), info.birthTime());

    // Using QFileSystemWatcher to monitor file changes is not
    // very reliable, particularly on Windows.  Even on linux,
    // there are cases where it's unreliable. So, we need to check
    // some other file attributes for changes to get more reliable
    // file monitoring.
    if(m_status == NoActivity) {
        if(!info.exists() || m_lastModification < lastRealModification || m_lastSize != info.size()) {
            m_status = FileChanged;
        }
    }

    if(m_status != NoActivity) {
        m_lastModification = lastRealModification;
        m_lastSize = info.size();

        if(info.exists()) {
            // Check if the file needs to be added to the
            // watcher regardless of whether m_status
            // was FileDeleted or FileChanged. The file
            // could have been deleted and recreated
            // before getting here.
            if(m_watcher->files().isEmpty()) {
                m_watcher->addPath(m_filename);
            }

            emit fileChanged();
            m_status = NoActivity;
        } else if(m_status != FileDeleted) {
            emit fileChanged();
            m_status = FileDeleted;
        }
    }
}

void YFileSystemWatcher::on_watcher_fileChanged(const QString & /*filepath*/)
{
    m_status = FileChanged;
}

