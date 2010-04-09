#include "FileBlockReader.h"
#include <QString>
#include <QTextStream>
#include <QtDebug>

FileBlockReader::FileBlockReader(const QString & filename)
	: m_file(filename)
{
	m_file.open(QIODevice::ReadOnly);
}

std::pair<qint64, qint64> FileBlockReader::readChunk(QString *data, qint64 start_pos, qint64 num_lines)
{
	while(start_pos > 0) {
		m_file.seek(start_pos - 1);
		char c = 0;
		m_file.peek(&c, 1);
		if(c == '\n') { break; }
		--start_pos;
	}

	m_file.seek(start_pos);

	QTextStream reader(&m_file);
	for(int i = 0; i < num_lines; i++) {
		*data += reader.readLine();
		if(i + 1 < num_lines) *data += "\n";
	}
	return std::pair<qint64,qint64>(start_pos,reader.pos());
}

qint64 FileBlockReader::size() const
{
	return m_file.size();
}
