target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  IpClientModule.hpp
  IpServerModule.hpp
)
#Client
add_library(cbtPosixIpClient
STATIC
  IpClientModule.cpp
)

target_link_libraries(cbtPosixIpClient PUBLIC abstractionLayer)
target_link_libraries(cbtPosixIpClient PUBLIC Network)
target_link_libraries(cbtPosixIpClient PUBLIC OperatingSystem)
target_link_libraries(cbtPosixIpClient PUBLIC Utilities)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtPosixIpClient)

#Server
add_library(cbtPosixIpServer
STATIC
  IpServerModule.cpp
)

target_link_libraries(cbtPosixIpServer PUBLIC abstractionLayer)
target_link_libraries(cbtPosixIpServer PUBLIC Network)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtPosixIpServer)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtPosixIpClient PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
target_compile_options(cbtPosixIpServer PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)