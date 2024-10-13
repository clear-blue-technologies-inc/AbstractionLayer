target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CellularModule.hpp
)

add_library(UbloxSaraU201Module
STATIC
  CellularModule.cpp
)

target_include_directories(UbloxSaraU201Module INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(UbloxSaraU201Module PUBLIC abstractionLayer)
target_link_libraries(UbloxSaraU201Module PUBLIC Gpio)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC UbloxSaraU201Module)

if (ESP_PLATFORM)
  target_include_directories(UbloxSaraU201Module PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(UbloxSaraU201Module PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
