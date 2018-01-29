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
