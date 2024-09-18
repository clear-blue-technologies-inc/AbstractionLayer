add_executable(cbtStorageTest.Mach-O
  CbtStorageTest.cpp
)

target_include_directories(cbtStorageTest.Mach-O
PRIVATE
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Storage
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/OperatingSystem
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Logging
  ${CMAKE_SOURCE_DIR}/../main/Foundation/Common

  ${CMAKE_SOURCE_DIR}/../main/Modules/Storage/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/OperatingSystem/Posix
)

find_library(storageLib
NAMES
  cbtStdLibStorage
HINTS
  ${buildDir}/main/Modules/Storage/StdLib
)

find_library(operatingSystemLib
NAMES
  cbtPosixOperatingSystem
HINTS
  ${buildDir}/main/Modules/OperatingSystem/Posix
)

find_library(errorLib
NAMES
  cbtErrnoError
HINTS
  ${buildDir}/main/Modules/Error/Errno
)

find_library(loggerLib
NAMES
  cbtStdlibLogger
HINTS
  ${buildDir}/main/Modules/Logging/StdLib
)

target_compile_options(cbtStorageTest.Mach-O PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(cbtStorageTest.Mach-O PRIVATE ${storageLib})
target_link_libraries(cbtStorageTest.Mach-O PRIVATE ${operatingSystemLib})
target_link_libraries(cbtStorageTest.Mach-O PRIVATE ${errorLib})
target_link_libraries(cbtStorageTest.Mach-O PRIVATE ${loggerLib})


add_test(
  NAME Storage
  COMMAND cbtStorageTest.Mach-O
)

set_property(TEST Storage
PROPERTY
  TIMEOUT 10
)