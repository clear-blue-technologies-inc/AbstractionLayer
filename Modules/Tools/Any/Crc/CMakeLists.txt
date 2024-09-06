target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CyclicRedundancyCheckModule.hpp
  crc.h
)

add_library(cbtAnyCyclicRedundancyCheck
STATIC
  CyclicRedundancyCheckModule.cpp
  crc.c
)

target_link_libraries(cbtAnyCyclicRedundancyCheck PUBLIC Utilities)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtAnyCyclicRedundancyCheck)

if (ESP_PLATFORM)
  target_include_directories(cbtAnyCyclicRedundancyCheck PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtAnyCyclicRedundancyCheck PUBLIC $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
endif()

target_compile_options(cbtAnyCyclicRedundancyCheck PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)