/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "BufferedFile.h"
#include <algorithm>
#include <QFileInfo>
#include <QFile>

const qint64 BUFFER_SIZE = 16384; // 16k

BufferedFile::BufferedFile(const QString & filename)
    : m_buffer_pos(-1)
    , m_size(0)
    , m_filename(filename)
{
    QFile file(m_filename);
    if(file.open(QIODevice::ReadOnly)) {
        m_size = file.size();
    } else {
        m_errorString = file.errorString();
    }
}

BufferedFile::~BufferedFile()
{
}

const QString & BufferedFile::filename() const
{
    return m_filename;
}

qint64 BufferedFile::size() const
{
    return m_size;
}

BufferedFile::Status BufferedFile::getChar(qint64 pos, char * ch)
{
    if(!m_errorString.isEmpty()) { return Error; }
    if(pos < 0 || pos >= m_size) { return Eof; }
    if(m_buffer.empty() || pos < m_buffer_pos || pos >= m_buffer_pos + BUFFER_SIZE) {
        m_buffer_pos = std::max(pos - (BUFFER_SIZE / 2), 0LL);
        qint64 size = std::min(BUFFER_SIZE, m_size - m_buffer_pos);
        QFile file(m_filename);
        if(file.open(QIODevice::ReadOnly)) {
            uchar * buffer = file.map(m_buffer_pos, size);
            m_buffer.assign(buffer, buffer + size);
            m_errorString = QString();
        } else {
            m_buffer.clear();
            m_errorString = file.errorString();
        }
    }

    if(ch && !m_buffer.empty()) {
       *ch = m_buffer[pos - m_buffer_pos];
       return Ok;
    }

    return Error;
}

const QString & BufferedFile::errorString() const
{
    return m_errorString;
}
