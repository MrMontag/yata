/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "BufferedFile.h"
#include <algorithm>

const qint64 BUFFER_SIZE = 16384; // 16k

BufferedFile::BufferedFile(const QString & filename)
    : m_buffer(0)
    , m_buffer_pos(-1)
    , m_size(0)
    , m_file(filename)
{
    m_file.open(QIODevice::ReadOnly);
    m_size = m_file.size();
}

BufferedFile::~BufferedFile()
{
}

qint64 BufferedFile::size() const
{
    return m_size;
}

bool BufferedFile::getChar(qint64 pos, char * ch)
{
    if(pos < 0 || pos >= m_size) { return false; }
    if(!m_buffer || pos < m_buffer_pos || pos >= m_buffer_pos + BUFFER_SIZE) {
        if(m_buffer) { m_file.unmap(m_buffer); }
        m_buffer_pos = std::max(pos - (BUFFER_SIZE / 2), 0LL);
        qint64 size = std::min(BUFFER_SIZE, m_size - m_buffer_pos);
        m_buffer = m_file.map(m_buffer_pos, size);
    }

    if(ch) {
       *ch = m_buffer[pos - m_buffer_pos];
       return true;
    }

    return false;
}

