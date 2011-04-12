#ifndef APPLICATION_SESSION_H
#define APPLICATION_SESSION_H

#include "SessionCommon.h"

#include <memory>
#include <string>
#include <vector>
#include <iostream>


namespace YAML {
    class Emitter;
    struct Mark;
    class Node;
}

struct FileSession;
struct SearchSession;

class ApplicationSession {
public:
    typedef std::vector<int> GContainer;

    static const std::string VERSION_KEY;
    static const std::string SEARCH_KEY;
    static const std::string FILE_INDEX_KEY;
    static const std::string FILE_KEY;
    static const std::string GEOMETRY_KEY;

    static const int VERSION_MAJOR = 1;
    static const int VERSION_MINOR = 0;

    ApplicationSession();
    ~ApplicationSession();
 
    void addFile(const FileSession & fileSession);
    void setSearch(const SearchSession & searchSession);

    size_t fileCount() const;
    const FileSession & fileAt(int index) const;
    const SearchSession & search() const;

    ParsingStatus::Enum status() const { return m_status; }
    void setStatus(ParsingStatus::Enum status) { m_status = status; }

    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int currentIndex) { m_currentIndex = currentIndex; }

    void setGeometry(GContainer & geometry);
    const GContainer & geometry() const;
private:
    std::auto_ptr<SearchSession> m_search;
    std::vector<FileSession> m_files;
    int m_currentIndex;
    ParsingStatus::Enum m_status;
    GContainer m_geometry;
};

YAML::Emitter & operator<<(YAML::Emitter & out, const ApplicationSession & appSession);
void operator>>(const YAML::Node & in, ApplicationSession & appSession);

#endif
