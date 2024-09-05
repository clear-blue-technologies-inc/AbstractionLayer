################################################################################
#Date: September 26th, 2023                                                    #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level project CMakeList.txt for AbstractionLayer desktop build  #
################################################################################

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  set(EXECUTABLE_SUFFIX ".Mach-O")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(EXECUTABLE_SUFFIX ".elf")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(EXECUTABLE_SUFFIX ".exe")
endif()

add_executable(${PROJECT_NAME}${EXECUTABLE_SUFFIX} "main/main.cpp")

set_target_properties(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (${RELEASE_BUILD})
  message(STATUS "Building Release build")
  target_compile_options(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PRIVATE
    -ffunction-sections
    -fno-exceptions
    -fdata-sections
    -Os
    -Wextra
    -std=gnu++23
  )

  target_compile_definitions(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC CONFIG_LOG_COLORS=0)

elseif (${DEBUG_BUILD})
  message(STATUS "Building Debug build")
  target_compile_options(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PRIVATE
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -O0
    -g3
    -ggdb
    -Wextra
    -std=gnu++23
  )

  target_compile_definitions(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC CONFIG_LOG_COLORS=1)

endif()

add_subdirectory(main/AbstractionLayer)

#Common code that has no code or platform dependencies other than the standard library.
add_subdirectory(main/Common)

##Application layer. Cross-platform implementation core code

#Module/Porting layer
#Modules are chunks of code that implement functionality. They are all interchangeable provided that the target platform
#supports the implementation.
#They serve as a method of porting to Foundation on different platforms.
#E.g. to run on Linux, replace or add main/Modules/Drivers/Linux/<module> subdirectory.
#To remove modules, use the "None" port since at least a function stub must exist for Foundation
#core code to call.
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Uart/MacOsCleonSimulator)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Watchdog/None)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Gpio/None)

add_subdirectory(main/AbstractionLayer/Modules/OperatingSystem/Posix)
add_subdirectory(main/AbstractionLayer/Modules/Network/Wifi/MacOs)
add_subdirectory(main/AbstractionLayer/Modules/Ip/Posix)
add_subdirectory(main/AbstractionLayer/Modules/Logging/Stdlib)
add_subdirectory(main/AbstractionLayer/Modules/Storage/Stdlib)
add_subdirectory(main/AbstractionLayer/Modules/Serialization/None)
add_subdirectory(main/AbstractionLayer/Modules/Error/Errno)
add_subdirectory(main/AbstractionLayer/Modules/Tools/Any/Crc)
add_subdirectory(main/AbstractionLayer/Modules/MemoryManagement/Default)
add_subdirectory(main/AbstractionLayer/Modules/Cryptography/MacOs)
add_subdirectory(main/AbstractionLayer/Utilities)
add_subdirectory(main/AbstractionLayer/Applications/Logging)
add_subdirectory(main/AbstractionLayer/Applications/ChainOfResponsibility)
