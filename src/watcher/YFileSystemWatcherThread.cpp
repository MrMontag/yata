/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
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
