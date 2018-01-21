/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FileBlockReader.h"
#include <QString>
#include <QTextStream>
#include <limits>

FileBlockReader::FileBlockReader(const QString & filename): m_file(filename)
{
}

bool FileBlockReader::readAll(QString * data, std::vector<qint64> * filePositions)
{
    return read(data, filePositions, 0, std::numeric_limits<qint64>::max());
}

bool FileBlockReader::readChunk(QString *data, std::vector<qint64> * filePositions,
    qint64 start_pos, qint64 lines_after_start, qint64 num_lines)
{
    start_pos = getStartPosition(start_pos, lines_after_start);
    if(start_pos < 0) { return false; }
    return read(data, filePositions, start_pos, num_lines);
}

bool FileBlockReader::readChunk(QString *data, qint64 start_pos, qint64 length)
{
    data->clear();

    char ch = 0;
    for(qint64 i = 0; i < length; i++) {
        BufferedFile::Status status = m_file.getChar(start_pos + i, &ch);
        if(status == BufferedFile::Error) { return false; }
        if(status == BufferedFile::Eof) { break; }
        data->append(ch);
    }
    return true;
}

qint64 FileBlockReader::getStartPosition(qint64 init_pos, qint64 lines_after_start)
{
    qint64 start_pos = beginningOfLine(init_pos);

    while(lines_after_start < 0 && start_pos > 0) {
        start_pos = beginningOfLine(start_pos - 1);
        if(start_pos < 0) { return -1; }
        ++lines_after_start;
    }

    while(lines_after_start > 0 && start_pos < size()) {
        start_pos = nextLine(start_pos);
        if(start_pos < 0) { return -1; }
        --lines_after_start;
    }

    return start_pos;
}

qint64 FileBlockReader::size() const
{
    return m_file.size();
}

const QString & FileBlockReader::filename() const
{
    return m_file.filename();
}

const QString & FileBlockReader::errorString() const
{
    return m_file.errorString();
}

qint64 FileBlockReader::beginningOfLine(qint64 start_pos)
{
    while(start_pos > 0) {
        char c = 0;
        BufferedFile::Status status = m_file.getChar(start_pos - 1, &c);
        if(status == BufferedFile::Error) { return -1; }
        if(status == BufferedFile::Eof || c == '\n') { break; }
        --start_pos;
    }
    return start_pos;
}

qint64 FileBlockReader::nextLine(qint64 start_pos)
{
    while(start_pos < size()) {
        ++start_pos;
        char c = 0;
        BufferedFile::Status status = m_file.getChar(start_pos - 1, &c);
        if(status == BufferedFile::Error) { return -1; }
        if(status == BufferedFile::Eof || c == '\n') { break; }
    }

    return start_pos;
}

bool FileBlockReader::read(QString * data, std::vector<qint64> * filePositions,
    qint64 start_pos, qint64 num_lines)
{
    data->clear();
    filePositions->clear();

    filePositions->push_back(start_pos);

    qint64 line_count = 0;

    // There is a bug in QString::append() that prevents it from interpreting
    // non-ASCII text values correctly when passed a char argument. The result is a
    // crash that occurs in YTextDocument::layoutBlock() when it calls
    // QTextLine::setLineWidth(). The crash does not occur when QString::append() is
    // called with a char * argument. Hence, even though we need a single character,
    // the following variable is declared as a char *.
    char ch[] = "\0";
    qint64 pos = start_pos;
    while(true) {
        BufferedFile::Status status = m_file.getChar(pos, ch);
        if(status == BufferedFile::Error) { return false; }
        if(status == BufferedFile::Eof) { break; }
        data->append(ch);
        pos++;
        if(ch[0] == '\n') {
            line_count++;
            if(line_count >= num_lines) {
                break;
            }
            filePositions->push_back(pos);
        }
    }
    return true;
}
