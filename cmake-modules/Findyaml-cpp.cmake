set(yaml-cpp_FOUND 0)

if(UNIX)
    set(YAML_CPP_INC_SEARCH_PATH
        /usr/local/include
        /usr/include)

    set(YAML_CPP_LIB_SEARCH_PATH
        /usr/local/lib
        /usr/lib)
    find_library(YAML_LIB yaml-cpp ${YAML_CPP_LIB_SEARCH_PATH})
else(UNIX)
if(WIN32)
    set(YAML_CPP_BASE_DIR C:/Program\ Files/YAML_CPP)
    set(YAML_CPP_INC_SEARCH_PATH ${YAML_CPP_BASE_DIR}/include)

    set(YAML_CPP_LIB_SEARCH_PATH ${YAML_CPP_BASE_DIR}/lib)
    find_library(YAML_LIB yaml-cpp ${YAML_CPP_LIB_SEARCH_PATH})
endif(WIN32)
endif(UNIX)

find_path(YAML_INCLUDE_PATH NAMES yaml-cpp/yaml.h PATHS ${YAML_CPP_INC_SEARCH_PATH})

if(YAML_INCLUDE_PATH AND YAML_LIB)
    set(yaml-cpp_FOUND 1)
    message("-- Found yaml-cpp")
endif()

if(NOT yaml-cpp_FOUND)
    if(yaml-cpp_FIND_REQUIRED)
        message(FATAL_ERROR "yaml-cpp not found")
    else()
        message(WARNING "yaml-cpp not found")
    endif()
endif()