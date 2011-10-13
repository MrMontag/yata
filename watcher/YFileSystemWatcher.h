/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YFILESYSTEMWATCHER_H
#define YFILESYSTEMWATCHER_H

#include <QObject>
#include <QDateTime>

class QFileSystemWatcher;
class QTimer;

class YFileSystemWatcher: public QObject {
    Q_OBJECT
public:
    YFileSystemWatcher(const QString & filename);
    ~YFileSystemWatcher();

signals:
    void fileChanged();

private slots:
    void on_timer_timeout();
    void on_watcher_fileChanged(const QString & filepath);
private:
    QScopedPointer<QFileSystemWatcher> m_watcher;
    QScopedPointer<QTimer> m_timer;

    enum Status {
        NoActivity,
        FileChanged,
        FileDeleted
    };

    Status m_status;
    QString m_filename;
    QDateTime m_lastModification;
    qint64 m_lastSize;

};

#endif // YFILESYSTEMWATCHER_H
