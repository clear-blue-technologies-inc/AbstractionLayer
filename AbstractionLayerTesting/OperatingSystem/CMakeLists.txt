add_executable(cbtOperatingSystemTest.Mach-O
  CbtOperatingSystemTest.cpp
)

target_include_directories(cbtOperatingSystemTest.Mach-O
PRIVATE
  ${CMAKE_SOURCE_DIR}/../main/AbstractionLayer/Storage
  ${CMAKE_SOURCE_DIR}/../main/AbstractionLayer/OperatingSystem
  ${CMAKE_SOURCE_DIR}/../main/AbstractionLayer/Logging
  ${CMAKE_SOURCE_DIR}/../main/Foundation/Common

  ${CMAKE_SOURCE_DIR}/../main/Modules/Storage/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/OperatingSystem/Posix
)

find_library(operatingSystemLib
NAMES
  cbtPosixOperatingSystem
HINTS
  ${buildDir}/main/Modules/OperatingSystem/Posix
)

target_compile_options(cbtOperatingSystemTest.Mach-O PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(cbtOperatingSystemTest.Mach-O PRIVATE ${operatingSystemLib})

add_test(
  NAME OperatingSystem
  COMMAND cbtOperatingSystemTest.Mach-O
)