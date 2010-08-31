#ifndef BUFFEREDFILE_H
#define BUFFEREDFILE_H

#include <QFile>


class BufferedFile {
public:
    BufferedFile(const QString & filename);
    ~BufferedFile();

    qint64 size() const;
    bool getChar(qint64 pos, char * ch);

private:
    uchar * m_buffer;
    qint64 m_buffer_pos;
    qint64 m_size;
    QFile m_file;
};

#endif // BUFFEREDFILE_H
