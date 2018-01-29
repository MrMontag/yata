/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QApplication>
#include <QDir>
#include <QScopedPointer>
#include <QIcon>
#include "gui/MainWindow.h"
#include "session/SessionLoader.h"
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
