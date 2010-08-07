#ifndef YFILESYSTEMWATCHERTHREAD_H
#define YFILESYSTEMWATCHERTHREAD_H

#include <QThread>
#include <QScopedPointer>

class QFileSystemWatcher;
class QTimer;

class YFileSystemWatcherThread : public QThread
{
Q_OBJECT
public:
    explicit YFileSystemWatcherThread(const QString & filename, QObject *parent = 0);
    ~YFileSystemWatcherThread();

    void stop();

protected:
    void run();

signals:
    void fileChanged();
    void fileDeleted();

private:
    QString m_filename;
};

#endif // YFILESYSTEMWATCHERTHREAD_H
