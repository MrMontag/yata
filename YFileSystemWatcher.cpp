#include "YFileSystemWatcher.h"

#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTimer>

YFileSystemWatcher::YFileSystemWatcher(const QString & filename)
    : m_watcher(new QFileSystemWatcher(this))
    , m_timer(new QTimer(this))
    , m_filename(filename)
{
    connect(m_watcher.data(), SIGNAL(fileChanged(QString)), SLOT(on_watcher_fileChanged(const QString &)));
    connect(m_timer.data(), SIGNAL(timeout()), SLOT(on_timer_timeout()));

    m_watcher->addPath(m_filename);

    QFileInfo info(m_filename);
    m_lastModification = info.lastModified();

    m_timer->start(250);
}

YFileSystemWatcher::~YFileSystemWatcher()
{
}

void YFileSystemWatcher::on_timer_timeout()
{
    QFileInfo info(m_filename);

    if(m_status == NoActivity) {
        if(!info.exists() || m_lastModification < info.lastModified()) {
            m_status = FileChanged;
        }
    }

    if(m_status != NoActivity) {
        m_lastModification = info.lastModified();

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
}

void YFileSystemWatcher::on_watcher_fileChanged(const QString & /*filepath*/)
{
    m_status = FileChanged;
}

