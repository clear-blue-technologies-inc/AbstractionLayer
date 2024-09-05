target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  UartModule.hpp
)

add_library(cbtEspUart
STATIC
  UartModule.cpp
)

target_include_directories(Uart INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cbtEspUart PUBLIC Uart)
target_link_libraries(cbtEspUart PUBLIC abstractionLayer)

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtEspUart)

if (ESP_PLATFORM)
  target_include_directories(cbtEspUart PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtEspUart PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
