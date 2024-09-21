target_sources(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CryptographyModule.hpp
)

add_library(LinuxCryptography
STATIC
  CryptographyModule.cpp
)
target_include_directories(Cryptography INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(LinuxCryptography PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libsodium-stable/src/libsodium/include)

target_link_libraries(LinuxCryptography PUBLIC Utilities)
target_link_libraries(LinuxCryptography PUBLIC abstractionLayer)
target_link_libraries(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC LinuxCryptography)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_LIST_DIR}/libsodium-stable/src/libsodium/.libs/libsodium.a
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/libsodium-stable
  COMMENT "Building libsodium..."
  COMMAND autoreconf -i && ./configure && make && make check
  VERBATIM
)
add_custom_target(libsodium-stable DEPENDS ${CMAKE_CURRENT_LIST_DIR}/libsodium-stable/src/libsodium/.libs/libsodium.a)
add_dependencies(LinuxCryptography libsodium-stable)
find_library(libsodium
NAMES
  sodium
HINTS
  libsodium-stable/src/libsodium/.libs/
)
target_link_libraries(LinuxCryptography PUBLIC ${libsodium})

if (ESP_PLATFORM)
  target_include_directories(LinuxCryptography PRIVATE $<TARGET_PROPERTY:__idf_main,INTERFACE_INCLUDE_DIRECTORIES>)
  target_include_directories(__idf_main PUBLIC ${CMAKE_CURRENT_LIST_DIR})
else()
  target_include_directories(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
endif()

target_compile_options(LinuxCryptography PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME}${EXECUTABLE_SUFFIX},COMPILE_OPTIONS>)
