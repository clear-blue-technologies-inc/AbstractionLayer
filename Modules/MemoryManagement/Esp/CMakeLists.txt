add_library(cbtEspMemoryConfig
STATIC
  MemoryConfigModule.cpp
)

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtEspMemoryConfig)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtEspMemoryConfig PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtEspMemoryConfig PUBLIC $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
endif()

target_compile_options(cbtEspMemoryConfig PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
