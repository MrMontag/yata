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
#ifndef FILEBLOCKREADER_H
#define FILEBLOCKREADER_H

#include <utility>
#include <QtGlobal>
#include <QFile>

#include <vector>

#include "BufferedFile.h"

class QString;

class FileBlockReader {
public:
    FileBlockReader(const QString & filename);

    bool readChunk(QString *data, std::vector<qint64> * filePositions,
        qint64 start_pos, qint64 lines_after_start, qint64 num_lines);
    bool readChunk(QString *data, qint64 start_pos, qint64 length);
    bool readAll(QString * data, std::vector<qint64> * filePositions);
    qint64 getStartPosition(qint64 init_pos, qint64 lines_after_start);

    qint64 size() const;
    const QString & filename() const;

    const QString & errorString() const;

private:
    qint64 beginningOfLine(qint64 start_pos);
    qint64 nextLine(qint64 start_pos);

    bool read(QString * data, std::vector<qint64> * filePositions, qint64 start_pos, qint64 num_lines);

private:
    BufferedFile m_file;
};
#endif
