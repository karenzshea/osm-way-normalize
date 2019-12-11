cmake_minimum_required(VERSION 3.5)
project(osm-way-normalize LANGUAGES CXX C)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -pedantic -std=c++17")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(PROTOZERO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/protozero/include")
include_directories(${PROTOZERO_INCLUDE_DIR})

set(OSMIUM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libosmium/include")
include_directories(${OSMIUM_INCLUDE_DIR})

find_package(Osmium 2.15.0 REQUIRED COMPONENTS io pbf xml)


if(NOT OSMIUM_FOUND)
    message(WARNING "Libosmium not found!\n")
endif()


include_directories(include)

set(SOURCES
    src/node_ref/node_ref_table.cpp
    src/tags/parse_tag_csv.cpp
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} ${OSMIUM_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} stdc++fs)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${OSMIUM_INCLUDE_DIRS})
