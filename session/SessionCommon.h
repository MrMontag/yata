#ifndef SESSION_COMMON_H
#define SESSION_COMMON_H

// TODO: move this to a yaml subdirectory, since it's being used by preferences serialization, not just session.

#include <yaml-cpp/yaml.h>
#include <string>

namespace ParsingStatus {
    enum Enum {
        OK,
        PossibleDataLoss,
        IncompatibleVersion,
        Error,
        FileNotFound
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
