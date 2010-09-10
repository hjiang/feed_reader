cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

Project(OnyxFeedReader CXX C)
enable_testing()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
SET(TEST_OUTPUT_PATH ${PROJECT_BINARY_DIR}/unittests)
SET(QT_PLUGINS_OUTPUT_PATH ${PROJECT_BINARY_DIR}/plugins)

include(enable_qt)
include(onyx_test)
include(strict_warning)
include(tc)

enable_qt()

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/sdk/code/include
  ${CMAKE_SOURCE_DIR}/third_party/gtest/include
  ${CMAKE_SOURCE_DIR}/third_party/gmock/include
)

if(BUILD_FOR_ARM)
  include(arm_toolchain)
  use_arm_toolchain()
  # Maximize window.
  add_definitions(-DSCREEN_WIDTH=-1 -DSCREEN_HEIGHT=-1 -DBUILD_FOR_ARM)
else(BUILD_FOR_ARM)

  # Use 600x800 to approximate device screen on PC.
  add_definitions(-DSCREEN_WIDTH=600)
  add_definitions(-DSCREEN_HEIGHT=800)
endif(BUILD_FOR_ARM)

IF(UNIX OR BUILD_FOR_ARM)
    ADD_DEFINITIONS(-DSHARE_ROOT="/usr/share")
ELSE (UNIX OR BUILD_FOR_ARM)
    ADD_DEFINITIONS(-DSHARE_ROOT="")
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF (UNIX OR BUILD_FOR_ARM)

set(ONYX_LIBS onyx_sys onyx_ui onyx_wireless onyx_wpa onyx_screen onyx_data
  onyx_cms)
ADD_DEFINITIONS(-DCONFIG_CTRL_IFACE)
IF (WIN32)
ADD_DEFINITIONS(-DCONFIG_NATIVE_WINDOWS)
ADD_DEFINITIONS(-DCONFIG_CTRL_IFACE_NAMED_PIPE)
ELSE (WIN32)
ADD_DEFINITIONS(-DCONFIG_CTRL_IFACE_UNIX)
ENDIF (WIN32)

add_subdirectory(src)
add_subdirectory(sdk/code)
set(feedreader_RCS feedreader.qrc)
if(NOT BUILD_FOR_ARM)
add_subdirectory(tests)
endif(NOT BUILD_FOR_ARM)
add_subdirectory(third_party)
