target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  GpioModule.hpp
)

add_library(EspGpio
STATIC
  GpioModule.cpp
)

target_include_directories(Gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(EspGpio PUBLIC abstractionLayer)
target_link_libraries(EspGpio PUBLIC Gpio)
target_link_libraries(EspGpio PUBLIC Logging)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC EspGpio)

if (ESP_PLATFORM)
  target_include_directories(EspGpio PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(EspGpio PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
