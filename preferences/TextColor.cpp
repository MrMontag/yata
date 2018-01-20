/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "TextColor.h"
#include <QApplication>
#include <yaml-cpp/yaml.h>

const std::string FOREGROUND_KEY = "foreground";
const std::string BACKGROUND_KEY = "background";

TextColor::TextColor():
    m_defaultForeground(QPalette::NoRole),
    m_defaultBackground(QPalette::NoRole)
{
}

TextColor::TextColor(const QColor & foreground, const QColor & background):
    m_foreground(new QColor(foreground)),
    m_background(new QColor(background)),
    m_defaultForeground(QPalette::NoRole),
    m_defaultBackground(QPalette::NoRole)
{
}

TextColor::TextColor(const TextColor & other):
    m_defaultForeground(other.m_defaultForeground),
    m_defaultBackground(other.m_defaultBackground)
{
    if(other.m_foreground.data()) { m_foreground.reset(new QColor(*other.m_foreground)); }
    if(other.m_background.data()) { m_background.reset(new QColor(*other.m_background)); }
}

TextColor::TextColor(QPalette::ColorRole defaultForeground, QPalette::ColorRole defaultBackground):
    m_defaultForeground(defaultForeground),
    m_defaultBackground(defaultBackground)
{
}

TextColor::~TextColor()
{
}

TextColor & TextColor::operator=(const TextColor & other)
{
    if (this == &other) { return *this; }
    if(other.m_foreground.data()) { m_foreground.reset(new QColor(*other.m_foreground)); }
    else { m_foreground.reset(); }

    if(other.m_background.data()) { m_background.reset(new QColor(*other.m_background)); }
    else { m_background.reset(); }
    m_defaultForeground = other.m_defaultForeground;
    m_defaultBackground = other.m_defaultBackground;
    return *this;
}

const QColor & TextColor::foreground() const
{
    if(m_foreground.data()) { return *m_foreground; }
    return QApplication::palette().color(m_defaultForeground);
}

void TextColor::setForeground(const QColor & color)
{
    m_foreground.reset(new QColor(color));
}

QPalette::ColorRole TextColor::defaultForeground() const
{
    return m_defaultForeground;
}

void TextColor::setDefaultForeground(QPalette::ColorRole colorRole)
{
    m_defaultForeground = colorRole;
}

const QColor & TextColor::background() const
{
    if(m_background.data()) { return *m_background; }
    return QApplication::palette().color(m_defaultBackground);
}

void TextColor::setBackground(const QColor & color)
{
    m_background.reset(new QColor(color));
}

QPalette::ColorRole TextColor::defaultBackground() const
{
    return m_defaultBackground;
}

void TextColor::setDefaultBackground(QPalette::ColorRole colorRole)
{
    m_defaultBackground = colorRole;
}

void TextColor::setToDefault(bool toDefault)
{
    if (toDefault) {
        m_foreground.reset();
        m_background.reset();
    } else {
        if (!m_foreground.data()) {
            m_foreground.reset(new QColor(QApplication::palette().color(m_defaultForeground)));
        }
        if (!m_background.data()) {
            m_background.reset(new QColor(QApplication::palette().color(m_defaultBackground)));
        }
    }
}

bool TextColor::isDefault() const
{
    return !m_foreground.data() && !m_background.data();
}

bool TextColor::isValid() const
{
    return m_foreground.data() && m_foreground->isValid() &&
        m_background.data() && m_background->isValid();
}

YAML::Emitter & operator<<(YAML::Emitter & out, const QColor & color)
{
    return out << YAML::Flow << YAML::BeginSeq << color.red() << color.green() << color.blue() << YAML::EndSeq;
}

void operator>>(const YAML::Node & in, QColor & color)
{
    if(in.size() == 3) {
        color.setRgb(in[0].as<int>(), in[1].as<int>(), in[2].as<int>());
        if(color.isValid()) { return; }
    }

    // Invalid input. Return default color.
    color = QColor();
}

YAML::Emitter & operator<<(YAML::Emitter & out, const TextColor & textColor)
{
    if(textColor.isDefault()) {
        return out << "default";
    }

    return out << YAML::BeginMap
        << YAML::Key << FOREGROUND_KEY << YAML::Value << textColor.foreground()
        << YAML::Key << BACKGROUND_KEY << YAML::Value << textColor.background()
        << YAML::EndMap;
}

void operator>>(const YAML::Node & in, TextColor & textColor)
{
    if(in.Type() == YAML::NodeType::Map) {
        textColor.setForeground(in[FOREGROUND_KEY].as<QColor>());
        textColor.setBackground(in[BACKGROUND_KEY].as<QColor>());
    }
}
