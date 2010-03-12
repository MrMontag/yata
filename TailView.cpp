#include "TailView.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <fstream>
#include <string>

TailView::TailView(QWidget * parent)
	: QWidget(parent)
{
	m_textView = new QTextEdit;
	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(m_textView);
	setLayout(layout);
}

void TailView::onFileChanged(const QString & path)
{
	std::string contents;
	std::ifstream input(path.toStdString().c_str());
	char c;
	while(input.get(c))
	{
		contents += c;
	}
	m_textView->setText(contents.c_str());
}

