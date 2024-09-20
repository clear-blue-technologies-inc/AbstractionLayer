target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CryptographyModule.hpp
)

add_library(EspCryptography
STATIC
  CryptographyModule.cpp
)
target_include_directories(Cryptography INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(EspCryptography PUBLIC abstractionLayer)
target_link_libraries(EspCryptography PUBLIC Cryptography)
target_link_libraries(EspCryptography PUBLIC Storage)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC EspCryptography)

if (ESP_PLATFORM)
  target_include_directories(EspCryptography PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(EspCryptography PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
