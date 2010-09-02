#include "FileBlockReader.h"
#include <QString>
#include <QTextStream>
#include <QtDebug>
#include <limits>

FileBlockReader::FileBlockReader(const QString & filename)
	: m_file(filename)
{
}

void FileBlockReader::readAll(QString * data, std::vector<qint64> * filePositions)
{
    read(data, filePositions, 0, std::numeric_limits<qint64>::max());
}

std::pair<qint64, qint64> FileBlockReader::readChunk(QString *data, std::vector<qint64> * filePositions,
    qint64 start_pos, qint64 lines_after_start, qint64 num_lines)
{
    start_pos = getStartPosition(start_pos, lines_after_start);
    return read(data, filePositions, start_pos, num_lines);
}

qint64 FileBlockReader::getStartPosition(qint64 init_pos, qint64 lines_after_start)
{
    qint64 start_pos = beginningOfLine(init_pos);

    while(lines_after_start < 0 && start_pos > 0) {
        start_pos = beginningOfLine(start_pos - 1);
        ++lines_after_start;
    }

    while(lines_after_start > 0 && start_pos < size()) {
        start_pos = nextLine(start_pos);
        --lines_after_start;
    }

    return start_pos;
}

qint64 FileBlockReader::size() const
{
	return m_file.size();
}

qint64 FileBlockReader::beginningOfLine(qint64 start_pos)
{
    while(start_pos > 0) {
        char c = 0;
        m_file.getChar(start_pos - 1, &c);
        if(c == '\n') { break; }
        --start_pos;
    }
    return start_pos;
}

qint64 FileBlockReader::nextLine(qint64 start_pos)
{
    while(start_pos < size()) {
        ++start_pos;
        char c;
        m_file.getChar(start_pos - 1, &c);
        if(c == '\n') {
            break;
        }
    }

    return start_pos;
}

std::pair<qint64, qint64> FileBlockReader::read(QString * data, std::vector<qint64> * filePositions,
    qint64 start_pos, qint64 num_lines)
{
    data->clear();
    filePositions->clear();

    filePositions->push_back(start_pos);

    qint64 line_count = 0;
    char ch;

    qint64 pos = start_pos;
    while(m_file.getChar(pos, &ch)) {
        data->append(ch);
        if(ch == '\n') {
            line_count++;
            if(line_count >= num_lines) {
                break;
            }
            filePositions->push_back(pos + 1);
        }
        pos++;
    }

    return std::pair<qint64,qint64>(start_pos, pos);
}
