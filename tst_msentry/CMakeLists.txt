find_package(Qt5 COMPONENTS REQUIRED Core Network Test)

set(CMAKE_AUTOMOC ON)

add_executable(tst_msentry tst_msentry.cpp)

target_link_libraries(tst_msentry msentry
  Qt5::Core
  Qt5::Network
  Qt5::Test
)

add_test(tst_msentry tst_msentry)
