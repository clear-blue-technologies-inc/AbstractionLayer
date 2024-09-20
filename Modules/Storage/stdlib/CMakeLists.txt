target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  FileModule.hpp
  StorageModule.hpp
)

add_library(StdlibStorage
STATIC
  StorageModule.cpp
  FileModule.cpp
)

target_include_directories(Storage INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(StdlibStorage PUBLIC abstractionLayer)
target_link_libraries(StdlibStorage PUBLIC Storage)
target_link_libraries(StdlibStorage PUBLIC Utilities)
target_link_libraries(StdlibStorage PUBLIC Event)
target_link_libraries(StdlibStorage PUBLIC Logging)
target_link_libraries(StdlibStorage PUBLIC LoggingApplication)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC StdlibStorage)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(StdlibStorage PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
