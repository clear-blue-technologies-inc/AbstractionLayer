add_executable(UartTest
  UartTest.cpp
)

target_include_directories(UartTest
PRIVATE
  ${CMAKE_SOURCE_DIR}/../Abstractions/Logging
  ${CMAKE_SOURCE_DIR}/../Abstractions/Hardware
  ${CMAKE_SOURCE_DIR}/../Abstractions/Storage
  ${CMAKE_SOURCE_DIR}/../Abstractions/OperatingSystem

  ${CMAKE_SOURCE_DIR}/../Modules/Logging/stdlib
  ${CMAKE_SOURCE_DIR}/../Modules/Storage/stdlib
  ${CMAKE_SOURCE_DIR}/../Modules/Drivers/Uart/File
)

find_library(errorLib
NAMES
  ErrnoError
HINTS
  ${buildDir}/main/Modules/Error/Errno
)

find_library(loggerLib
NAMES
  StdlibLogger
HINTS
  ${buildDir}/AbstractionLayer/Modules/Logging/stdlib
)

find_library(uartLib
NAMES
  FileUart
HINTS
  ${buildDir}/AbstractionLayer/Modules/Drivers/Uart/File
)

find_library(storageLib
NAMES
  StdlibStorage
HINTS
  ${buildDir}/AbstractionLayer/Modules/Storage/Stdlib/StdlibStorage
)

find_library(operatingSystemLib
NAMES
  ${CMAKE_HOST_SYSTEM_NAME}OperatingSystem
HINTS
  ${buildDir}/AbstractionLayer/Modules/OperatingSystem/${CMAKE_HOST_SYSTEM_NAME}
)

target_compile_options(UartTest PRIVATE $<TARGET_PROPERTY:abstractionLayerTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(UartTest PRIVATE ${errorLib})
target_link_libraries(UartTest PRIVATE ${loggerLib})
target_link_libraries(UartTest PRIVATE ${uartLib})
target_link_libraries(UartTest PRIVATE ${storageLib})
target_link_libraries(UartTest PRIVATE ${operatingSystemLib})

add_test(
  NAME Uart
  COMMAND UartTest
)

set_property(TEST Uart
PROPERTY
  TIMEOUT 10
)