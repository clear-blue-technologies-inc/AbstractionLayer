target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  IpClientModule.hpp
)
#Client
add_library(cbtEspIp
STATIC
  IpClientModule.cpp
)

target_include_directories(IpClient INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cbtEspIp PUBLIC abstractionLayer)
target_link_libraries(cbtEspIp PUBLIC IpClient)
target_link_libraries(cbtEspIp PUBLIC Logging)
target_link_libraries(cbtEspIp PUBLIC Utilities)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtEspIp)

if (ESP_PLATFORM)
  target_include_directories(cbtEspIp PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtEspIp PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
