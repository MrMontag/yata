#ifndef BUFFEREDFILE_H
#define BUFFEREDFILE_H

#include <QIODevice>
#include <QString>


class BufferedFile: public QIODevice {
public:
    BufferedFile(const QString & filename);
    ~BufferedFile();

    virtual qint64 pos() const;
    virtual bool seek(qint64 pos);
    virtual qint64 size() const;
    virtual bool open(OpenMode mode);
protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    qint64 m_pos;
    qint64 m_size;
    char * m_buffer;
    QString m_filename;
};

#endif // BUFFEREDFILE_H
