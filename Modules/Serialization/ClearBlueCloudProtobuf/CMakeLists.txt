target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  SerializationModule.hpp
)

add_library(cbtProtobufSerialization
STATIC
  SerializationModule.cpp
)

target_include_directories(Serialization INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cbtProtobufSerialization PUBLIC abstractionLayer)
target_link_libraries(cbtProtobufSerialization PUBLIC Serialization)

add_library(nanopb
STATIC
  cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86/pb_common.c
  cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86/pb_decode.c
  cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86/pb_encode.c
)

add_library(cbtCleonFoundationProtobufApi
STATIC
  cleon-foundation-api/generated/api_fc.pb.c
  cleon-foundation-api/generated/api_cf.pb.c
  cleon-foundation-api/generated/apiF0tx.pb.c
)

add_library(cbtCloudFoundationProtobufApi
STATIC
  v2-cloud-api/generated/apiF0rx.pb.c
  v2-cloud-api/generated/apiF0tx.pb.c
  v2-cloud-api/generated/apiA0.pb.c
  v2-cloud-api/generated/apiBA.pb.c
  v2-cloud-api/generated/apiF1.pb.c
)

#Since the Cmake docs aren't very good at explaining the concepts (IMO), the PUBLIC and PRIVATE keywords control whether this
#include will be propogated to other targets when nanopb is linked. PUBLIC includes will be seen by other targets when this is linked
#and added to their INCLUDE_DIRECTORIES property. PRIVATE includes will only be seen by this target.

#You can make some arguments about whether PUBLIC includes are good practice. You can argue against by saying PRIVATE include forces
#you to be more explicit about which headers are included in which targets by forcing you to write a target_include_directories for it
#instead of just linking it. I would say the larger the scope, the better it is to be more explicit. Since the scope of each
#CMakeLists.txt is small, I don't think it's too confusing to use a PUBLIC include to save on a few lines.
target_include_directories(nanopb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86)
target_include_directories(cbtCleonFoundationProtobufApi PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cleon-foundation-api/generated)
target_include_directories(cbtCleonFoundationProtobufApi PRIVATE $<TARGET_PROPERTY:nanopb,INCLUDE_DIRECTORIES>)

target_include_directories(cbtCloudFoundationProtobufApi PRIVATE $<TARGET_PROPERTY:nanopb,INCLUDE_DIRECTORIES>)
target_include_directories(cbtCloudFoundationProtobufApi PUBLIC ${CMAKE_CURRENT_LIST_DIR}/v2-cloud-api/generated)

target_include_directories(cbtProtobufSerialization PRIVATE $<TARGET_PROPERTY:cbtCleonFoundationProtobufApi,INCLUDE_DIRECTORIES>)

target_link_libraries(cbtProtobufSerialization PUBLIC nanopb)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtProtobufSerialization)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtCleonFoundationProtobufApi)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtCloudFoundationProtobufApi)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtProtobufSerialization PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)