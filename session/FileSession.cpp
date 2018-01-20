#include "FileSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>

const std::string FileSession::PATH_KEY = "path";
const std::string FileSession::ADDRESS_KEY = "address";
const std::string FileSession::FOLLOW_TAIL_KEY = "follow-tail";

YAML::Emitter & operator<<(YAML::Emitter & out, const FileSession & session)
{
    out << YAML::BeginMap;
    out << YAML::Key << FileSession::PATH_KEY << YAML::Value << session.path;
    out << YAML::Key << FileSession::ADDRESS_KEY << YAML::Value << session.address;
    out << YAML::Key << FileSession::FOLLOW_TAIL_KEY << YAML::Value << session.followTail;
    out << YAML::EndMap;
    return out;
}

void operator>>(const YAML::Node & in, FileSession & session)
{
    session.path = getValue<std::string>(in, FileSession::PATH_KEY);
    session.address = getValue<long long>(in, FileSession::ADDRESS_KEY);
    session.followTail = getValue<bool>(in, FileSession::FOLLOW_TAIL_KEY);
}
