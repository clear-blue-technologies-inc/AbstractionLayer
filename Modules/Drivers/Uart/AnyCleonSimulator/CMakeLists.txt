target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  UartModule.hpp
)

add_library(cbtMacOsCleonSimulatorUart
STATIC
  UartModule.cpp
)

target_include_directories(Uart INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC abstractionLayer)
target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC Storage)
target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC Uart)
target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC Utilities)
target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC Event)
target_link_libraries(cbtMacOsCleonSimulatorUart PUBLIC Serialization)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtMacOsCleonSimulatorUart)

target_include_directories(cbtMacOsCleonSimulatorUart PUBLIC "../../../Serialization/ClearBlueCloudProtobuf/cleon-foundation-api/generated")
target_include_directories(cbtMacOsCleonSimulatorUart PUBLIC "../../../Serialization/ClearBlueCloudProtobuf/cleon-foundation-api/vendors/nanopb/nanopb-0.4.8-linux-x86")
target_include_directories(cbtMacOsCleonSimulatorUart PUBLIC "../../../Tools/Any/Crc")

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtMacOsCleonSimulatorUart PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)