/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "Preferences.h"
#include "YApplication.h"
#include <QFont>
#include <yaml-cpp/yaml.h>
#include <fstream>

// TODO: implement versioning for preference files

Preferences * Preferences::m_instance;

const std::string FONT_KEY = "font";

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
    emitter << YAML::Key << FONT_KEY << YAML::Value << m_font->toString().toStdString();
    emitter << YAML::EndMap;

    std::ofstream out(YApplication::preferencesFilePath().toStdString().c_str());
    out << emitter.c_str();
}

void Preferences::read()
{
    try {
        std::ifstream in(YApplication::preferencesFilePath().toStdString().c_str());
        YAML::Parser parser(in);
        YAML::Node document;
        if (parser.GetNextDocument(document)) {
            if (const YAML::Node * font = document.FindValue(FONT_KEY)) {
                std::string fontStr;
                *font >> fontStr;
                m_font->fromString(fontStr.c_str());
            }
        }
    } catch(YAML::Exception & ex) {
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

Preferences::Preferences():
    m_font(new QFont())
{
}

Preferences::~Preferences()
{
    m_instance = 0;
}
