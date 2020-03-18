if (POLICY CMP0048)
        cmake_policy(SET CMP0048 NEW)
endif ()

cmake_minimum_required(VERSION 3.5)

project(msentry
  VERSION 0.0.1
  DESCRIPTION "Milo Sentry client"
  LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(SOURCES msentry.h raven.h ravendefs.h msentry.cpp raven.cpp ravendefs.cpp)

set(OTHER_FILES README.md LICENSE msentry.doxyfile)

add_library(msentry OBJECT ${SOURCES} ${OTHER_FILES})

target_compile_features(msentry PRIVATE cxx_alias_templates)

option (sentry "MSentry: sentry support" OFF)
if (sentry)
  target_compile_definitions(msentry PUBLIC SENTRY)

  option (sentry-stacktrace "MSentry: stack trace support" OFF)
  if (sentry-stacktrace)
    target_compile_definitions(msentry PUBLIC QRAVEN_STACKTRACE)
  endif ()

  option (sentry-print "MSentry: print messages to console in addition to sending it to sentry server" OFF)
  if (sentry-print)
    target_compile_definitions(msentry PUBLIC SENTRY_PRINT)
  endif ()
endif ()

target_include_directories(msentry
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(msentry
  Qt5::Core
  Qt5::Network
)

add_subdirectory(tst_msentry)
