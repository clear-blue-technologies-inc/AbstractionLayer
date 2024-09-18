add_executable(ExampleTest
  ExampleTest.cpp
)

target_include_directories(ExampleTest
PRIVATE
  ${CMAKE_SOURCE_DIR}/../Abstractions/Logging
  ${CMAKE_SOURCE_DIR}/../Utilities
  ${CMAKE_SOURCE_DIR}/../Modules/Error/Errno
  ${CMAKE_SOURCE_DIR}/../Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../Applications/Logger
  ${CMAKE_SOURCE_DIR}/../Applications/Event
)

find_library(errorLib
NAMES
  ErrnoError
HINTS
  ${buildDir}/main/Modules/Error/Errno
)

#find_library(loggerLib
#NAMES
#  StdlibLogger
#HINTS
#  ${buildDir}/main/Modules/Logging/StdLib
#)
#
#target_compile_options(ExampleTest PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)
#
#target_link_libraries(ExampleTest PRIVATE ${errorLib})
#target_link_libraries(ExampleTest PRIVATE ${loggerLib})

add_test(
  NAME Example
  COMMAND ExampleTest
)

set_property(TEST Example
PROPERTY
  TIMEOUT 10
)