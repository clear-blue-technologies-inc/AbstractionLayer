target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  LoggerModule.hpp
)

add_library(EspLogger
STATIC
  LoggerModule.cpp
)

target_include_directories(Logging INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(EspLogger PUBLIC abstractionLayer)
target_link_libraries(EspLogger PUBLIC Logging)
target_link_libraries(EspLogger PUBLIC Event)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC EspLogger)

if (ESP_PLATFORM)
  target_include_directories(EspLogger PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(EspLogger PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
