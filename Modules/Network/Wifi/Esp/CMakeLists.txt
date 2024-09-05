target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  WifiModule.hpp
)

add_library(cbtEspWifi
STATIC
  WifiModule.cpp
)

target_include_directories(Network INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cbtEspWifi PUBLIC abstractionLayer)
target_link_libraries(cbtEspWifi PUBLIC Network)
target_link_libraries(cbtEspWifi PUBLIC Common)
target_link_libraries(cbtEspWifi PUBLIC Logging)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtEspWifi)

if (ESP_PLATFORM)
  target_include_directories(cbtEspWifi PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtEspWifi PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
