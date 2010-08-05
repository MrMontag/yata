#ifndef YFILESYSTEMWATCHER_H
#define YFILESYSTEMWATCHER_H

#include <QThread>

class QFileSystemWatcher;
class QTimer;

class YFileSystemWatcher : public QThread
{
Q_OBJECT
public:
    explicit YFileSystemWatcher(const QString & filename, QObject *parent = 0);
    ~YFileSystemWatcher();

    void stop();

protected:
    void run();

signals:
    void fileChanged();
    void fileDeleted();

private slots:
    void on_timer_timeout();
    void on_watcher_fileChanged(const QString & filepath);
private:
    QFileSystemWatcher * m_watcher;
    QTimer * m_timer;

    enum Status {
        NoActivity,
        FileChanged,
        FileDeleted
    };

    Status m_status;
    QString m_filename;
};

#endif // YFILESYSTEMWATCHER_H
