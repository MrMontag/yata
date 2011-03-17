#ifndef SESSION_COMMON_H
#define SESSION_COMMON_H

#include <yaml-cpp/yaml.h>
#include <string>

template <typename T> T getValue(const YAML::Node & in, const std::string & key)
{
    if(const YAML::Node *node = in.FindValue(key)) {
        T t;
        *node >> t;
        return t;
    } else {
        return T();
    }
}

namespace ParsingStatus {
    enum Enum {
        OK,
        PossibleDataLoss,
        IncompatibleVersion,
        Error,
        FileNotFound,
    };
}

struct ParsingError {
    int line;
    int column;
    std::string message;

    ParsingError(int l, int n, const std::string & m)
        : line(l)
        , column(n)
        , message(m)
    {
    }
};

#endif
