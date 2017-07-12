cmake_minimum_required(VERSION 2.6.2)

SET(CMAKE_C_COMPILER /usr/local/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/g++)

project(RobotController)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

set(MODULE_NAME RobotController)
add_definitions(-DMODULE_NAME=\"RobotController\")

add_definitions(-Wall)
add_definitions(-fPIC)


set(RobotController_VERSION_MAJOR 0)
set(RobotController_VERSION_MINOR 1)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

################################################################################
find_package(DanLib REQUIRED)

include_directories(${DanLib_INCLUDE_DIR})
message(" - DanLib found include dir = ${DanLib_INCLUDE_DIR}")

################################################################################
find_package(VideoLib REQUIRED)

include_directories(${VideoLib_INCLUDE_DIR})
message(" - VideoLib found include dir = ${VideoLib_INCLUDE_DIR}")
################################################################################
find_package(GuiStuff REQUIRED)

include_directories(${GuiStuff_INCLUDE_DIR})

message(" - GuiStuff found include dir = ${GuiStuff_INCLUDE_DIR}")
################################################################################
find_package(wxWidgets REQUIRED)

include("${wxWidgets_USE_FILE}")

################################################################################
add_executable(
  RobotController
  WIN32
  src/RobotControllerApp.cpp
  )

target_link_libraries(
  RobotController
  GuiStuffLib
  VideoPlayerLib
  Image
  ${wxWidgets_LIBRARIES}
  )

################################################################################
# Install
################################################################################
set(RobotController_VERSION_STRING "${RobotController_VERSION_MAJOR}.${RobotController_VERSION_MINOR}")
