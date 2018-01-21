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
