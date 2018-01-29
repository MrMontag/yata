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
#ifndef YAMLCONVERTERS_H
#define YAMLCONVERTERS_H

#include "TextColor.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

template<>
struct convert<QColor>
{
    static Node encode(const QColor &color) {
        Node node;
        node.push_back(color.rgb());
        return node;
    }

    static bool decode(const Node &node, QColor &color) {
        if (!node.IsScalar() || node.size() != 1)
            return false;
        color.setRgb(node.as<unsigned int>());
        return true;
    }
};

template<>
struct convert<TextColor>
{
    // TODO QPalette::ColorRole
    static Node encode(const TextColor &textColor) {
        Node node;
        node.push_back(textColor.foreground());
        node.push_back(textColor.background());
        return node;
    }

    static bool decode(const Node &node, TextColor &textColor) {
        if (!node.IsSequence() || node.size() != 2)
            return false;
        textColor.setForeground(QColor::fromRgb(node[0].as<unsigned int>()));
        textColor.setBackground(QColor::fromRgb(node[1].as<unsigned int>()));
        return true;
    }
};

}

#endif // YAMLCONVERTERS_H
