#ifndef FILEBLOCKREADER_H
#define FILEBLOCKREADER_H

#include <utility>
#include <QtGlobal>
#include <QFile>

class QString;

class FileBlockReader {
public:
	FileBlockReader(const QString & filename);

	std::pair<qint64, qint64> readChunk(QString *data, qint64 start_pos, qint64 num_lines);
	qint64 size() const;
private:
	QFile m_file;
};
#endif
