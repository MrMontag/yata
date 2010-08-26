#include "BufferedFile.h"
#include <QFile>

BufferedFile::BufferedFile(const QString & filename)
    : m_pos(0)
    , m_size(0)
    , m_buffer(0)
    , m_filename(filename)
{
}

BufferedFile::~BufferedFile()
{
    delete [] m_buffer;
    m_buffer = 0;
}

qint64 BufferedFile::size() const
{
    return m_size;
}

bool BufferedFile::open(OpenMode mode)
{
    QFile file(m_filename);
    if(QIODevice::open(mode) && file.open(mode)) {
        m_size = file.size();
        m_buffer = new char[m_size];
        file.read(m_buffer, m_size);
        return true;
    }

    return false;
}

qint64 BufferedFile::pos() const
{
    return m_pos;
}

bool BufferedFile::seek(qint64 pos)
{
    if(QIODevice::seek(pos)) {
        if(pos < size()) {
            m_pos = pos;
            return true;
        }
    }

    return false;
}

qint64 BufferedFile::readData(char *data, qint64 maxlen)
{
    if(m_pos >= size()) { return -1; }

    qint64 tocopy = std::min<qint64>(maxlen, size() - m_pos);

    memcpy(data, m_buffer + m_pos, tocopy);
    m_pos += tocopy;
    return tocopy;
}

qint64 BufferedFile::writeData(const char * /*data*/, qint64 /*len*/)
{
    return -1;
}
