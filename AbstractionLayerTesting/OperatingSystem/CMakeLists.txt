add_executable(OperatingSystemTest
  OperatingSystemTest.cpp
)

target_include_directories(OperatingSystemTest
PRIVATE
  ${CMAKE_SOURCE_DIR}/../Abstractions/Storage
  ${CMAKE_SOURCE_DIR}/../Abstractions/OperatingSystem
  ${CMAKE_SOURCE_DIR}/../Abstractions/Logging

  ${CMAKE_SOURCE_DIR}/../Modules/Storage/stdlib
  ${CMAKE_SOURCE_DIR}/../Modules/Logging/stdlib
  ${CMAKE_SOURCE_DIR}/../Modules/OperatingSystem/MacOs
)

find_library(operatingSystemLib
NAMES
  MacOsOperatingSystem
HINTS
  ${buildDir}/AbstractionLayer/Modules/OperatingSystem/MacOs
)

target_compile_options(OperatingSystemTest PRIVATE $<TARGET_PROPERTY:abstractionLayerTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(OperatingSystemTest PRIVATE ${operatingSystemLib})

add_test(
  NAME OperatingSystem
  COMMAND OperatingSystemTest
)
