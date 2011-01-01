/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include <QApplication>
#include <QScopedPointer>
#include "MainWindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/smiley.png"));
    QScopedPointer<MainWindow> win(new MainWindow);
    for(int i = 1; i < argc; i++) {
        win->addFile(argv[i]);
    }

    win->show();
    return app.exec();
}
