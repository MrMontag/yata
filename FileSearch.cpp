#include "FileSearch.h"
#include "FileBlockReader.h"

#include <QRegExp>
#include <QtDebug>

FileSearch::FileSearch(const QString & filename)
    : m_filename(filename)
{
}

const FileCursor & FileSearch::cursor() const
{
    return m_fileCursor;
}

void FileSearch::setCursor(const FileCursor & c)
{
    m_fileCursor = c;
}

bool FileSearch::searchFile(bool isForward, bool wrapAround)
{
    if(lastSearchString().isEmpty()) {
        m_fileCursor.m_linePosition = -1;
        m_fileCursor.m_lineOffset = 0;
        m_fileCursor.m_length = 0;
        return false;
    }

    if(m_fileCursor.isNull()) {
        m_fileCursor = FileCursor(0,0,0);
    }

    qint64 initialLinePosition = m_fileCursor.m_linePosition;

    QRegExp regex(getRegex());
    FileBlockReader reader(m_filename);

    int lineOffset = 0;

    bool initialLoop = true;

    while(true) {
        if(isMatch(regex, &m_fileCursor, &reader, lineOffset, isForward)) {
            qDebug() << m_fileCursor.m_linePosition << m_fileCursor.m_lineOffset << m_fileCursor.m_length;
            return true;
        }

        if(isForward) {
            lineOffset = 1;
            m_fileCursor.m_lineOffset = 0;
            if(m_fileCursor.m_linePosition >= reader.size() - 1) {
                if(!wrapAround) { return false; }
                m_fileCursor.m_linePosition = 0;
                lineOffset = 0;
            }
        } else {
            lineOffset = -1;
            m_fileCursor.m_lineOffset = -1;
            if(m_fileCursor.m_linePosition == 0) {
                if(!wrapAround) { return false; }
                m_fileCursor.m_linePosition = reader.size();
                lineOffset = 0;
            }
        }

        if(!initialLoop && m_fileCursor.m_linePosition == initialLinePosition) {
            return false;
        }

        initialLoop = false;
    }
}

bool FileSearch::isMatch(const QRegExp & regex, FileCursor * cursor, FileBlockReader * reader, int lineOffset, bool isForward)
{
    std::vector<qint64> linePositions;
    QString data;

    reader->readChunk(&data, &linePositions, cursor->m_linePosition, lineOffset, 1);

    if(cursor->m_lineOffset == -1) {
        cursor->m_lineOffset = data.length();
    }

    if(linePositions.empty()) {
        return false;
    }

    int startPos = -1;
    int matchLength = 0;
    if(isForward) {
        startPos = regex.indexIn(data, cursor->m_lineOffset + cursor->m_length);
        matchLength = regex.matchedLength();
    } else {
        int pos = 0;
        while(true) {
            pos = regex.indexIn(data, pos + matchLength);
            if(pos == -1) {
                break;
            }

            if(pos + regex.matchedLength() < cursor->m_lineOffset) {
                startPos = pos;
                matchLength = regex.matchedLength();
            } else {
                break;
            }
        }
    }

    cursor->m_linePosition = linePositions.front();

    if(startPos != -1) {
        cursor->m_lineOffset = startPos;
        cursor->m_length = matchLength;
        return true;
    } else {
        cursor->m_lineOffset = -1;
        cursor->m_length = 0;
        return false;
    }
}
