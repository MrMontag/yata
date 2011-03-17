set(yaml-cpp_FOUND 0)

set(YAML_CPP_INC_SEARCH_PATH
    /usr/local/include
    /usr/include)

set(YAML_CPP_LIB_SEARCH_PATH
    /usr/local/lib
    /usr/lib)

find_path(YAML_INCLUDE_PATH yaml-cpp/yaml.h ${YAML_CPP_INC_SEARCH_PATH})
find_library(YAML_LIB yaml-cpp ${YAML_CPP_LIB_SEARCH_PATH})

if(YAML_INCLUDE_PATH AND YAML_LIB)
    set(yaml-cpp_FOUND 1)
endif()

if(yaml-cpp_FOUND)
else()
    if(yaml-cpp_FIND_REQUIRED)
        message(FATAL_ERROR "yaml-cpp not found")
    else()
        message(WARNING "yaml-cpp not found")
    endif()
endif()
    
