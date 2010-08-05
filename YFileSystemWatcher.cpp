#include "YFileSystemWatcher.h"

#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTimer>

YFileSystemWatcher::YFileSystemWatcher(const QString & filename, QObject *parent) :
    QThread(parent),
    m_watcher(new QFileSystemWatcher(this)),
    m_timer(new QTimer(this)),
    m_status(NoActivity),
    m_filename(filename)
{
    start();
}

YFileSystemWatcher::~YFileSystemWatcher()
{
    stop();
}

void YFileSystemWatcher::stop()
{
    quit();
    wait();
}

void YFileSystemWatcher::run()
{
    connect(m_watcher, SIGNAL(fileChanged(QString)), SLOT(on_watcher_fileChanged(const QString &)));
    connect(m_timer, SIGNAL(timeout()), SLOT(on_timer_timeout()));

    m_watcher->addPath(m_filename);
    m_timer->start(250);
    exec();
}

void YFileSystemWatcher::on_timer_timeout()
{
    if(m_status == NoActivity) { return; }

    QFileInfo info(m_filename);
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
        emit fileDeleted();
        m_status = FileDeleted;
    }
}

void YFileSystemWatcher::on_watcher_fileChanged(const QString & /*filepath*/)
{
    m_status = FileChanged;
}
