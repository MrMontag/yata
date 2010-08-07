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
    connect(&fsw, SIGNAL(fileDeleted()), SIGNAL(fileDeleted()));

    exec();
}
