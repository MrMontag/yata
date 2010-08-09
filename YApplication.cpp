#include <QObject>
#include <QString>
#include "YApplication.h"

QString YApplication::displayAppName()
{
    return QObject::tr("Yata");
}
