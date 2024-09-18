################################################################################
#Date: May 9th, 2024                                                           #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level project CMakeList.txt for testing foundation              #
################################################################################
cmake_minimum_required(VERSION 3.5)

project(foundationTesting)

include(CTest)

#The name of the build folder for desktop builds changes based on the operating system used.
file(GLOB buildDir ${CMAKE_SOURCE_DIR}/../*_build)

add_library(foundationTesting INTERFACE)

target_compile_options(foundationTesting
INTERFACE
  -ffunction-sections
  -fdata-sections
  -fno-exceptions
  -O0
  -g3
  -ggdb
  -Wextra
  -std=gnu++23
)

add_subdirectory(Example)
#add_subdirectory(OperatingSystem)
#add_subdirectory(Storage)
#add_subdirectory(Ip)
#add_subdirectory(Uart)
#add_subdirectory(Serialization)
