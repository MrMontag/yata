/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include <QApplication>
#include <QDir>
#include <QScopedPointer>
#include <QIcon>
#include "MainWindow.h"
#include "SessionLoader.h"
#include "YApplication.h"
#include "preferences/Preferences.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/yata.png"));
    Preferences::instance()->read();
    QScopedPointer<MainWindow> win(new MainWindow);

    QDir dir;
    dir.mkpath(YApplication::settingsPath());

    SessionLoader::readSession(win.data());

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            win->addFile(argv[i]);
        }
        if(win->fileCount() > 0) {
            win->setCurrentFileIndex(win->fileCount() - 1);
        }
    }

    win->show();
    return app.exec();
}
