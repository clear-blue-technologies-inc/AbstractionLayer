add_library(cbtNoneError
STATIC
  ErrorModule.cpp
)

target_include_directories(Network INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Uart INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Storage INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(OperatingSystem INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC cbtNoneError)

if (ESP_PLATFORM)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtNoneError PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(cbtNoneError PUBLIC $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},INCLUDE_DIRECTORIES>)
endif()

target_compile_options(cbtNoneError PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
