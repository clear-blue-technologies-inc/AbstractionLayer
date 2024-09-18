add_executable(cbtIpTest.Mach-O
  CbtIpTest.cpp
)

target_include_directories(cbtIpTest.Mach-O
PRIVATE
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Ip
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/OperatingSystem
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Logging
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Network

  ${CMAKE_SOURCE_DIR}/../main/Foundation/Common
  ${CMAKE_SOURCE_DIR}/../main/Foundation/Protocols

  ${CMAKE_SOURCE_DIR}/../main/Modules/Ip/Posix
  ${CMAKE_SOURCE_DIR}/../main/Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/OperatingSystem/Posix
  ${CMAKE_SOURCE_DIR}/../main/Modules/Network/Wifi/MacOs
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

find_library(wifiLib
NAMES
  cbtMacOsWifi
HINTS
  ${buildDir}/main/Modules/Network/Wifi/MacOs
)

find_library(ipClientLib
NAMES
  cbtPosixIpClient
HINTS
  ${buildDir}/main/Modules/Ip/Posix
)

find_library(ipServerLib
NAMES
  cbtPosixIpServer
HINTS
  ${buildDir}/main/Modules/Ip/Posix
)

target_compile_options(cbtIpTest.Mach-O PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(cbtIpTest.Mach-O PRIVATE ${storageLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${operatingSystemLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${errorLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${loggerLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${wifiLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${ipClientLib})
target_link_libraries(cbtIpTest.Mach-O PRIVATE ${ipServerLib})

add_test(
  NAME Ip
  COMMAND cbtIpTest.Mach-O
)

set_property(TEST Ip
PROPERTY
  TIMEOUT 10
)