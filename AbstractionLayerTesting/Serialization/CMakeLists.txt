add_executable(cbtSerializationTest.Mach-O
  CbtSerializationTest.cpp
)

target_include_directories(cbtSerializationTest.Mach-O
PRIVATE
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Logging
  ${CMAKE_SOURCE_DIR}/../main/Foundation/AbstractionLayer/Serialization

  ${CMAKE_SOURCE_DIR}/../main/Foundation/Common

  ${CMAKE_SOURCE_DIR}/../main/Modules/Logging/Stdlib
  ${CMAKE_SOURCE_DIR}/../main/Modules/Serialization/Protobuf
  ${CMAKE_SOURCE_DIR}/../main/Modules/Serialization/Protobuf/cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86
  ${CMAKE_SOURCE_DIR}/../main/Modules/Serialization/Protobuf/cleon-foundation-api/generated
  ${CMAKE_SOURCE_DIR}/../main/Modules/Serialization/Protobuf/v2-cloud-api/generated
)

find_library(serializationLib
NAMES
  cbtProtobufSerialization
HINTS
  ${buildDir}/main/Modules/Serialization/Protobuf
)

find_library(nanopb
NAMES
  nanopb
HINTS
  ${buildDir}/main/Modules/Serialization/Protobuf
)

find_library(v2ProtobufCloudApi
NAMES
  cbtCloudFoundationProtobufApi
HINTS
  ${buildDir}/main/Modules/Serialization/Protobuf
)

find_library(v2ProtobufCleonApi
NAMES
  cbtCleonFoundationProtobufApi
HINTS
  ${buildDir}/main/Modules/Serialization/Protobuf
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

target_compile_options(cbtSerializationTest.Mach-O PRIVATE $<TARGET_PROPERTY:foundationTesting,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${errorLib})
target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${loggerLib})
target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${serializationLib})
target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${nanopb})
target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${v2ProtobufCleonApi})
target_link_libraries(cbtSerializationTest.Mach-O PRIVATE ${v2ProtobufCloudApi})

add_test(
  NAME Serialization
  COMMAND cbtSerializationTest.Mach-O
)

set_property(TEST Serialization
PROPERTY
  TIMEOUT 10
)