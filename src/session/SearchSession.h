#ifndef SEARCH_SESSION_H
#define SEARCH_SESSION_H

#include <string>

namespace YAML {
    class Emitter;
    class Node;
}

struct SearchSession {
    std::string expression;
    bool isRegex;
    bool isCaseSensitive;

    static const std::string EXPRESSION_KEY;
    static const std::string IS_REGEX_KEY;
    static const std::string IS_CASE_SENSITIVE_KEY;

    SearchSession(std::string e = std::string(), bool r = false, bool cs = false)
        : expression(e)
        , isRegex(r)
        , isCaseSensitive(cs)
    {
    }
};

YAML::Emitter & operator<<(YAML::Emitter & out, const SearchSession & session);
void operator>>(const YAML::Node & in, SearchSession & session);

#endif
