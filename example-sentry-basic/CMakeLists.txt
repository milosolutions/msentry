find_package(Qt5 COMPONENTS REQUIRED Core Network)

set(CMAKE_AUTOMOC ON)

add_executable(example-sentry-basic main.cpp)

target_link_libraries(example-sentry-basic msentry
  Qt5::Core Qt5::Network
)
