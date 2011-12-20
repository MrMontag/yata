/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "TextColor.h"
#include "session/SessionCommon.h"
#include <QColor>
#include <yaml-cpp/yaml.h>

const std::string FOREGROUND_KEY = "foreground";
const std::string BACKGROUND_KEY = "background";

TextColor::TextColor():
    m_foreground(new QColor),
    m_background(new QColor)
{
}

TextColor::TextColor(const QColor & foreground, const QColor & background):
    m_foreground(new QColor(foreground)),
    m_background(new QColor(background))
{
}

TextColor::TextColor(const TextColor & other):
    m_foreground(new QColor(*other.m_foreground)),
    m_background(new QColor(*other.m_background))
{
}

TextColor::~TextColor()
{
}

TextColor & TextColor::operator=(const TextColor & other)
{
    if (&other == this) { return *this; }
    *m_foreground = *other.m_foreground;
    *m_background = *other.m_background;
    return *this;
}

const QColor & TextColor::foreground() const
{
    return *m_foreground;
}

void TextColor::setForeground(const QColor & color) const
{
    *m_foreground = color;
}

const QColor & TextColor::background() const
{
    return *m_background;
}

void TextColor::setBackground(const QColor & color) const
{
    *m_background = color;
}

YAML::Emitter & operator<<(YAML::Emitter & out, const QColor & color)
{
    return out << YAML::Flow << YAML::BeginSeq << color.red() << color.green() << color.blue() << YAML::EndSeq;
}

void operator>>(const YAML::Node & in, QColor & color)
{
    std::vector<int> rgb;
    in >> rgb;
    if(rgb.size() == 3) {
        color.setRgb(rgb[0], rgb[1], rgb[2]);
        if(color.isValid()) { return; }
    }

    // Invalid input. Return default color.
    color = QColor();
}

YAML::Emitter & operator<<(YAML::Emitter & out, const TextColor & textColor)
{
    return out << YAML::BeginMap
        << YAML::Key << FOREGROUND_KEY << YAML::Value << textColor.foreground()
        << YAML::Key << BACKGROUND_KEY << YAML::Value << textColor.background()
        << YAML::EndMap;
}

void operator>>(const YAML::Node & in, TextColor & textColor)
{
    textColor.setForeground(getValue<QColor>(in, FOREGROUND_KEY));
    textColor.setBackground(getValue<QColor>(in, BACKGROUND_KEY));
}
