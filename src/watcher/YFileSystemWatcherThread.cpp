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
#include "YFileSystemWatcherThread.h"
#include "YFileSystemWatcher.h"


YFileSystemWatcherThread::YFileSystemWatcherThread(const QString & filename, QObject *parent)
    : QThread(parent)
    , m_filename(filename)
{
    start();
}

YFileSystemWatcherThread::~YFileSystemWatcherThread()
{
    stop();
}

void YFileSystemWatcherThread::stop()
{
    quit();
    wait();
}

void YFileSystemWatcherThread::run()
{
    YFileSystemWatcher fsw(m_filename);
    connect(&fsw, SIGNAL(fileChanged()), SIGNAL(fileChanged()));

    exec();
}
