target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  WatchdogModule.hpp
)

add_library(cbtNoneWatchdog
STATIC
  WatchdogModule.cpp
)

target_include_directories(Watchdog INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cbtNoneWatchdog PUBLIC abstractionLayer)
target_link_libraries(cbtNoneWatchdog PUBLIC Watchdog)
target_link_libraries(cbtNoneWatchdog PUBLIC Common)
target_link_libraries(cbtNoneWatchdog PUBLIC Logging)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtNoneWatchdog)

if (ESP_PLATFORM)
  target_include_directories(cbtNoneWatchdog PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(cbtNoneWatchdog PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)