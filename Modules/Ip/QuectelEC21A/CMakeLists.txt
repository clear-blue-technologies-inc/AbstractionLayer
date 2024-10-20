target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  IpClientModule.hpp
  IpServerModule.hpp
)
#Client
add_library(QuectelEC21AIpClient
STATIC
  IpClientModule.cpp
)

target_link_libraries(QuectelEC21AIpClient PUBLIC abstractionLayer)
target_link_libraries(QuectelEC21AIpClient PUBLIC IpClient)
target_link_libraries(QuectelEC21AIpClient PUBLIC Logging)
target_link_libraries(QuectelEC21AIpClient PUBLIC Utilities)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC QuectelEC21AIpClient)

#Server
add_library(QuectelEC21AIpServer
STATIC
  IpServerModule.cpp
)

target_link_libraries(QuectelEC21AIpServer PUBLIC abstractionLayer)
target_link_libraries(QuectelEC21AIpServer PUBLIC IpClient)
target_link_libraries(QuectelEC21AIpServer PUBLIC Logging)
target_link_libraries(QuectelEC21AIpServer PUBLIC Utilities)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC QuectelEC21AIpServer)

if (ESP_PLATFORM)
  target_include_directories(QuectelEC21AIpServer PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(QuectelEC21AIpClient PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(QuectelEC21AIpServer PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
target_compile_options(QuectelEC21AIpClient PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)