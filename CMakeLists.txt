cmake_minimum_required(VERSION 3.8)

project(eventbus)

include(GNUInstallDirs)

file(GLOB_RECURSE eventbus_HEADERS "${CMAKE_CURRENT_LIST_DIR}/Source/*.hpp")

message(${eventbus_HEADERS})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_DEMO ON)

add_library(eventbus INTERFACE ${eventbus_HEADERS})

target_include_directories(eventbus INTERFACE Source/.)

add_executable(eventbus_demo "Demo/main.cpp")

target_link_libraries(eventbus_demo PUBLIC eventbus)

install(
    TARGETS eventbus_demo
)

if(${BUILD_DEMO})

endif()