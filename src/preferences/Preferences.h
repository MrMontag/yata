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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QScopedPointer>

class QFont;
class TextColor;
namespace YAML { class Node; }

class Preferences: public QObject {
    Q_OBJECT
public:
    static Preferences * instance();

    void write();
    void read();

    const QFont & font() const;
    void setFont(const QFont & font);

    const TextColor & normalTextColor() const;
    void setNormalTextColor(const TextColor & color) const;

    const TextColor & selectedTextColor() const;
    void setSelectedTextColor(const TextColor & color);

    bool debugMenu() const;

    ~Preferences();

signals:
    void preferencesChanged();

private:
    Preferences();

    void readColor(TextColor * dest, const YAML::Node & src, const std::string & key);
private:
    QScopedPointer<QFont> m_font;
    QScopedPointer<TextColor> m_normalTextColor;
    QScopedPointer<TextColor> m_selectedTextColor;
    QScopedPointer<bool> m_debugMenu;

    static Preferences * m_instance;
};

#endif // PREFERENCES_H
