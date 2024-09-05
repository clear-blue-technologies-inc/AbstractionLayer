target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  Global.hpp
)

add_library(Utilities INTERFACE)

target_include_directories(Utilities INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} INTERFACE Utilities)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()