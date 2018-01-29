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
