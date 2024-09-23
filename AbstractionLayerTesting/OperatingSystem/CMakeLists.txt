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
  ${CMAKE_SOURCE_DIR}/../Modules/OperatingSystem/${CMAKE_HOST_SYSTEM_NAME}
)

find_library(operatingSystemLib
NAMES
  ${CMAKE_HOST_SYSTEM_NAME}OperatingSystem
HINTS
  ${buildDir}/AbstractionLayer/Modules/OperatingSystem/${CMAKE_HOST_SYSTEM_NAME}
)

target_compile_options(OperatingSystemTest PRIVATE $<TARGET_PROPERTY:abstractionLayerTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(OperatingSystemTest PRIVATE ${operatingSystemLib})

add_test(
  NAME OperatingSystem
  COMMAND OperatingSystemTest
)