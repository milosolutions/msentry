cmake_minimum_required(VERSION 2.8.11)

project(qraven)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(raven main.cpp raven.cpp)

target_compile_features(raven PRIVATE cxx_alias_templates)

target_link_libraries(raven Qt5::Core Qt5::Network)


