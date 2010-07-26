#ifndef FILEBLOCKREADER_H
#define FILEBLOCKREADER_H

#include <utility>
#include <QtGlobal>
#include <QFile>

#include <vector>

class QString;

class FileBlockReader {
public:
	FileBlockReader(const QString & filename);

    std::pair<qint64, qint64> readChunk(QString *data, std::vector<qint64> * filePositions,
        qint64 start_pos, qint64 lines_before_start, qint64 num_lines);
    qint64 getStartPosition(qint64 init_pos, qint64 lines_after_start);

    void readAll(QString * data, std::vector<qint64> * filePositions);

	qint64 size() const;
private:
    qint64 beginningOfLine(qint64 start_pos);
    qint64 nextLine(qint64 start_pos);

    std::pair<qint64, qint64> read(QString * data, std::vector<qint64> * filePositions,
        qint64 start_pos, qint64 num_lines);

private:
	QFile m_file;
};
#endif
