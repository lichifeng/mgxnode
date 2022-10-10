cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
 
project (mgxnode)

# Main lib generate by this script
aux_source_directory(./ ADDON_SRC)
add_library(${PROJECT_NAME} SHARED ${ADDON_SRC} ${CMAKE_JS_SRC} $<TARGET_OBJECTS:MgxParser>)

# Integrate MgxParser
set(NODE_ADDON 1)
set(MGXPARSER_DIR libs/MgxParser/)
add_subdirectory(${MGXPARSER_DIR})
file(COPY "${MGXPARSER_DIR}/src/include/MgxParser.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Fix include issues in vscode(`node_api.h not found` issue)
file(COPY "${CMAKE_JS_INC}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${NODE_ADDON_API_DIR})

# define NAPI_VERSION
add_definitions(-DNAPI_VERSION=3)
