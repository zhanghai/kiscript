cmake_minimum_required(VERSION 3.3)

project(kiscript)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Werror")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(BoehmGC REQUIRED)
find_package(GLIB REQUIRED)

set(SOURCE_FILES garbage_collection.c utils.c glib_utils.c parser.c
        lexical_parser_utils.c lexical_parser.c syntactic_parser_utils.c
        expression_parser.c statement_parser.c function_parser.c
        program_parser.c syntactic_parser.c interpreter.c main.c)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${BOEHMGC_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${BOEHMGC_LIBRARIES}
        ${GLIB_LIBRARIES})
