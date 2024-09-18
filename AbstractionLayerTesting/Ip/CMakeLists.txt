add_executable(IpTest
  IpTest.cpp
)

target_include_directories(IpTest
PRIVATE
  ${CMAKE_SOURCE_DIR}/../Abstractions/Ip
  ${CMAKE_SOURCE_DIR}/../Abstractions/OperatingSystem
  ${CMAKE_SOURCE_DIR}/../Abstractions/Logging
  ${CMAKE_SOURCE_DIR}/../Abstractions/Network
  ${CMAKE_SOURCE_DIR}/../Abstractions/Protocols

  ${CMAKE_SOURCE_DIR}/../Modules/Ip/Posix
  ${CMAKE_SOURCE_DIR}/../Modules/Logging/stdlib
  ${CMAKE_SOURCE_DIR}/../Modules/OperatingSystem/MacOs
  ${CMAKE_SOURCE_DIR}/../Modules/Network/Wifi/MacOs

  ${CMAKE_SOURCE_DIR}/../Utilities

  ${CMAKE_SOURCE_DIR}/../Applications/Event
  ${CMAKE_SOURCE_DIR}/../Applications/Logging
)

find_library(operatingSystemLib
NAMES
  MacOsOperatingSystem
HINTS
  ${buildDir}/AbstractionLayer/Modules/OperatingSystem/MacOs
)

find_library(errorLib
NAMES
  ErrnoError
HINTS
  ${buildDir}/AbstractionLayer/Modules/Error/Errno
)

find_library(loggerLib
NAMES
  StdlibLogger
HINTS
  ${buildDir}/AbstractionLayer/Modules/Logging/stdLib
)

find_library(wifiLib
NAMES
  MacOsWifi
HINTS
  ${buildDir}/AbstractionLayer/Modules/Network/Wifi/MacOs
)

find_library(ipClientLib
NAMES
  PosixIpClient
HINTS
  ${buildDir}/AbstractionLayer/Modules/Ip/Posix
)

find_library(ipServerLib
NAMES
  PosixIpServer
HINTS
  ${buildDir}/AbstractionLayer/Modules/Ip/Posix
)

find_library(eventLib
NAMES
  Event
HINTS
  ${buildDir}/AbstractionLayer/Applications/Event
)

target_compile_options(IpTest PRIVATE $<TARGET_PROPERTY:abstractionLayerTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(IpTest PRIVATE ${operatingSystemLib})
target_link_libraries(IpTest PRIVATE ${errorLib})
target_link_libraries(IpTest PRIVATE ${loggerLib})
target_link_libraries(IpTest PRIVATE ${wifiLib})
target_link_libraries(IpTest PRIVATE ${ipClientLib})
target_link_libraries(IpTest PRIVATE ${ipServerLib})
target_link_libraries(IpTest PRIVATE ${eventLib})

add_test(
  NAME Ip
  COMMAND IpTest
)

set_property(TEST Ip
PROPERTY
  TIMEOUT 10
)
