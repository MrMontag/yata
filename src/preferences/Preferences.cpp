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
#include "preferences/Preferences.h"
#include "YApplication.h"
#include "TextColor.h"
#include "YAMLConverters.h"

#include <QFont>
#include <fstream>

// TODO: implement versioning for preference files

Preferences * Preferences::m_instance;

const std::string TEXT_KEY = "text";
const std::string FONT_KEY = "font";
const std::string NORMAL_KEY = "normal";
const std::string SELECTED_KEY = "selected";
const std::string DEBUG_MENU_KEY = "debug-menu";

Preferences * Preferences::instance()
{
    if (!m_instance) {
        m_instance = new Preferences();
    }
    return m_instance;
}

void Preferences::write()
{
    YAML::Emitter emitter;

    emitter << YAML::BeginMap;
    emitter << YAML::Key << TEXT_KEY << YAML::Value;

    emitter << YAML::BeginMap;
    emitter << YAML::Key << FONT_KEY << YAML::Value << m_font->toString().toStdString();
    emitter << YAML::Key << NORMAL_KEY << YAML::Value << *m_normalTextColor;
    emitter << YAML::Key << SELECTED_KEY << YAML::Value << *m_selectedTextColor;
    emitter << YAML::EndMap;

    if(m_debugMenu.data()) {
        emitter << YAML::Key << DEBUG_MENU_KEY << YAML::Value << *m_debugMenu;
    }

    emitter << YAML::EndMap;

    std::ofstream out(YApplication::preferencesFilePath().toStdString().c_str());
    out << emitter.c_str();
}

void Preferences::read()
{
    // TODO review
    try {
        std::ifstream in(YApplication::preferencesFilePath().toStdString().c_str());
        YAML::Node document = YAML::Load(in);
        if (document.IsDefined()) {
            auto text = document[TEXT_KEY];
            if (text.IsDefined()) {
                std::string fontStr = text[FONT_KEY].as<std::string>();
                m_font->fromString(fontStr.c_str());

                readColor(m_normalTextColor.data(), text, NORMAL_KEY);
                readColor(m_selectedTextColor.data(), text, SELECTED_KEY);
            }
            auto debugMenuNode = document[DEBUG_MENU_KEY];
            if (debugMenuNode.IsDefined()) {
                bool debugMenu = debugMenuNode.as<bool>();
                m_debugMenu.reset(new bool(debugMenu));
            }
        }
    } catch(YAML::Exception & ex) {
        // TODO: handle parsing errors
    }
}

const QFont & Preferences::font() const
{
    return *m_font;
}

void Preferences::setFont(const QFont & font)
{
    m_font.reset(new QFont(font));
    emit preferencesChanged();
}

const TextColor & Preferences::normalTextColor() const
{
    return *m_normalTextColor;
}

void Preferences::setNormalTextColor(const TextColor & color) const
{
    Q_ASSERT(m_normalTextColor->defaultForeground() == color.defaultForeground());
    Q_ASSERT(m_normalTextColor->defaultBackground() == color.defaultBackground());
    *m_normalTextColor = color;
}

const TextColor & Preferences::selectedTextColor() const
{
    return *m_selectedTextColor;
}

void Preferences::setSelectedTextColor(const TextColor & color)
{
    Q_ASSERT(m_selectedTextColor->defaultForeground() == color.defaultForeground());
    Q_ASSERT(m_selectedTextColor->defaultBackground() == color.defaultBackground());
    *m_selectedTextColor = color;
}

bool Preferences::debugMenu() const
{
    return m_debugMenu.data() ? *m_debugMenu : false;
}

Preferences::Preferences():
    m_font(new QFont()),
    m_normalTextColor(new TextColor(QPalette::Text, QPalette::Base)),
    m_selectedTextColor(new TextColor(QPalette::HighlightedText, QPalette::Highlight))
{
}

Preferences::~Preferences()
{
    m_instance = 0;
}

void Preferences::readColor(TextColor * dest, const YAML::Node & src, const std::string & key)
{
    TextColor color = src[key].as<TextColor>();
    if(color.isValid()) {
        dest->setForeground(color.foreground());
        dest->setBackground(color.background());
    } else {
        dest->setToDefault();
    }
}
