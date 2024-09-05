target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CyclicRedundancyCheckModule.hpp
)

add_library(cbtNoneCyclicRedundancyCheck
STATIC
  CyclicRedundancyCheckModule.cpp
)

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtNoneCyclicRedundancyCheck)

if (ESP_PLATFORM)
  target_include_directories(cbtNoneCyclicRedundancyCheck PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtNoneCyclicRedundancyCheck PUBLIC $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
endif()

target_compile_options(cbtNoneCyclicRedundancyCheck PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
