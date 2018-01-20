/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
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
