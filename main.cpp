#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/smiley.png"));
	MainWindow * win = new MainWindow;
	if(argc > 1) {
		win->addFile(argv[1]);
	}
	win->show();
	return app.exec();
}
