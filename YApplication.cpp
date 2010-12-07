/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include <QObject>
#include <QString>
#include "YApplication.h"

QString YApplication::displayAppName()
{
    return QObject::tr("Yata");
}
