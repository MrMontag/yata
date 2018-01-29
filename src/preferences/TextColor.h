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
#ifndef TEXTCOLOR_H
#define TEXTCOLOR_H

#include <QScopedPointer>
#include <QPalette>

namespace YAML {
    class Emitter;
    class Node;
}

class TextColor {
public:
    TextColor(const QColor & foreground, const QColor & background);
    TextColor(QPalette::ColorRole defaultForeground, QPalette::ColorRole defaultBackground);
    TextColor(const TextColor & other);
    TextColor();
    ~TextColor();

    TextColor & operator=(const TextColor & other);

    const QColor & foreground() const;
    void setForeground(const QColor &);
    QPalette::ColorRole defaultForeground() const;
    void setDefaultForeground(QPalette::ColorRole colorRole);

    const QColor & background() const;
    void setBackground(const QColor &);
    QPalette::ColorRole defaultBackground() const;
    void setDefaultBackground(QPalette::ColorRole colorRole);

    void setToDefault(bool toDefault = true);
    bool isDefault() const;
    bool isValid() const;
private:
    QScopedPointer<QColor> m_foreground;
    QScopedPointer<QColor> m_background;
    QPalette::ColorRole m_defaultForeground;
    QPalette::ColorRole m_defaultBackground;
};

YAML::Emitter & operator<<(YAML::Emitter & out, const TextColor & textColor);
void operator>>(const YAML::Node & in, TextColor & textColor);

YAML::Emitter & operator<<(YAML::Emitter & out, const QColor & color);
void operator>>(const YAML::Node & in, QColor & color);

#endif // TEXTCOLOR_H
