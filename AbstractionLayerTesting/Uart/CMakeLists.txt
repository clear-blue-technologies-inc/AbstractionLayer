add_executable(cbtUartTest.Mach-O
  CbtUartTest.cpp
)

target_include_directories(cbtUartTest.Mach-O
PRIVATE
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Logging
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Hardware
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Storage
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/OperatingSystem

  ${CMAKE_SOURCE_DIR}/../main/Foundation/Common
  ${CMAKE_SOURCE_DIR}/../main/Foundation/Protocols

  ${CMAKE_SOURCE_DIR}/../main/Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/Storage/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/Drivers/Uart/StdlibFile
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

find_library(uartLib
NAMES
  cbtStdlibFileUart
HINTS
  ${buildDir}/main/Modules/Drivers/Uart/StdlibFile
)

find_library(storageLib
NAMES
  cbtStdlibStorage
HINTS
  ${buildDir}/main/Modules/Storage/Stdlib/StdlibStorage
)

find_library(operatingSystemLib
NAMES
  cbtPosixOperatingSystem
HINTS
  ${buildDir}/main/Modules/OperatingSystem/Posix
)

target_compile_options(cbtUartTest.Mach-O PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(cbtUartTest.Mach-O PRIVATE ${errorLib})
target_link_libraries(cbtUartTest.Mach-O PRIVATE ${loggerLib})
target_link_libraries(cbtUartTest.Mach-O PRIVATE ${uartLib})
target_link_libraries(cbtUartTest.Mach-O PRIVATE ${storageLib})
target_link_libraries(cbtUartTest.Mach-O PRIVATE ${operatingSystemLib})

add_test(
  NAME Uart
  COMMAND cbtUartTest.Mach-O
)

set_property(TEST Uart
PROPERTY
  TIMEOUT 10
)