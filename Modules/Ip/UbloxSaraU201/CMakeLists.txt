target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  IpCellularClientModule.hpp
  IpCellularServerModule.hpp
)
#Client
add_library(UbloxSaraU201IpCellularClient
STATIC
  IpCellularClientModule.cpp
)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC UbloxSaraU201IpCellularClient)

#Server
add_library(UbloxSaraU201IpCellularServer
STATIC
  IpCellularServerModule.cpp
)

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC UbloxSaraU201IpCellularServer)

#TODO: I should consider doing all my CMakeFiles like this. I could split up the folders into interfaces and include everything like this.
#I could make it so ${PROJECT_NAME}${EXECUTABLE_SUFFIX} doesn't have everything, but maybe 3 or 4 interfaces have all the headers you need.
if (ESP_PLATFORM)
  target_include_directories(UbloxSaraU201IpCellularClient PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(UbloxSaraU201IpCellularServer PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(UbloxSaraU201IpCellularClient PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
  target_include_directories(UbloxSaraU201IpCellularServer PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(UbloxSaraU201IpCellularClient PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
target_compile_options(UbloxSaraU201IpCellularServer PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
