#include "FileBlockReader.h"
#include <QString>
#include <QTextStream>
#include <QtDebug>

FileBlockReader::FileBlockReader(const QString & filename)
	: m_file(filename)
{
	m_file.open(QIODevice::ReadOnly);
}

std::pair<qint64, qint64> FileBlockReader::readChunk(QString *data, qint64 start_pos, qint64 lines_after_start, qint64 num_lines)
{
    data->clear();

    start_pos = getStartPosition(start_pos, lines_after_start);

    m_file.seek(start_pos);

    qint64 line_count = 0;
    char ch;
    while(m_file.getChar(&ch)) {
        data->append(ch);
        if(ch == '\n') {
            line_count++;
            if(line_count >= num_lines) {
                break;
            }
        }
    }

    return std::pair<qint64,qint64>(start_pos, m_file.pos());
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
        m_file.seek(start_pos - 1);
        char c = 0;
        m_file.peek(&c, 1);
        if(c == '\n') { break; }
        --start_pos;
    }
    return start_pos;
}

qint64 FileBlockReader::nextLine(qint64 start_pos)
{
    while(start_pos < size()) {
        ++start_pos;
        m_file.seek(start_pos - 1);
        char c;
        m_file.peek(&c, 1);
        if(c == '\n') {
            break;
        }
    }
    return start_pos;
}
