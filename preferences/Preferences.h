/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QScopedPointer>

class QFont;

class Preferences: public QObject {
    Q_OBJECT
public:
    static Preferences * instance();

    const QFont & font() const;
    void setFont(const QFont & font);

    ~Preferences();

signals:
    void preferencesChanged();

private:
    Preferences();

private:
    QScopedPointer<QFont> m_font;

    static Preferences * m_instance;
};

#endif // PREFERENCES_H
