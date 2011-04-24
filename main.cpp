/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include <QApplication>
#include <QDir>
#include <QScopedPointer>
#include "MainWindow.h"
#include "SessionLoader.h"
#include "YApplication.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/yata.png"));
    QScopedPointer<MainWindow> win(new MainWindow);

    QDir dir;
    dir.mkpath(YApplication::settingsPath());

    // TODO: When search sessions is implemented, it should still be read in when files are
    // specified on the command line.
    if (argc > 1) {
        for(int i = 1; i < argc; i++) {
            win->addFile(argv[i]);
        }
    } else {
        SessionLoader::readSession(win.data());
    }

    win->show();
    return app.exec();
}
