#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	MainWindow * win = new MainWindow;
	if(argc > 1) {
		win->setFile(argv[1]);
	}
	win->show();
	return app.exec();
}
