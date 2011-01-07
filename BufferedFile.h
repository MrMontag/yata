/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef BUFFEREDFILE_H
#define BUFFEREDFILE_H

#include <QString>
#include <vector>

class BufferedFile {
public:
    BufferedFile(const QString & filename);
    ~BufferedFile();

    qint64 size() const;
    bool getChar(qint64 pos, char * ch);

private:
    std::vector<uchar> m_buffer;
    qint64 m_buffer_pos;
    qint64 m_size;
    QString m_filename;
};

#endif // BUFFEREDFILE_H
