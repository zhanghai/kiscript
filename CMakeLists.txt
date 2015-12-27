cmake_minimum_required(VERSION 3.3)

project(kiscript)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GLIB REQUIRED)

set(SOURCE_FILES main.c lexical_parser.c lexical_parser.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLIB_LIBRARIES})
