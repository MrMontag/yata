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
