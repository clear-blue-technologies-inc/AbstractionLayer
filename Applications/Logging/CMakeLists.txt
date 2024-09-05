target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  Log.hpp
)

add_library(LoggingApplication INTERFACE)

target_include_directories(LoggingApplication INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC Logging)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC LoggingApplication)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()
