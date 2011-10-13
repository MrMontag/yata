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
    enum Status {
        Ok,
        Eof,
        Error
    };

    BufferedFile(const QString & filename);
    ~BufferedFile();

    const QString & filename() const;

    qint64 size() const;
    Status getChar(qint64 pos, char * ch);
    const QString & errorString() const;

private:
    std::vector<uchar> m_buffer;
    qint64 m_buffer_pos;
    qint64 m_size;
    QString m_filename;
    QString m_errorString;
};

#endif // BUFFEREDFILE_H
