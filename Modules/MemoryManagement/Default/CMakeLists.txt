add_library(cbtDefaultMemoryConfig
STATIC
  MemoryConfigModule.cpp
)

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtDefaultMemoryConfig)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtDefaultMemoryConfig PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtDefaultMemoryConfig PUBLIC $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
endif()

target_compile_options(cbtDefaultMemoryConfig PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)