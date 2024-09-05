################################################################################
#Date: April 17th, 2023                                                        #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level project CMakeList.txt for AbstractionLayer ESP build      #
################################################################################

message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  set(EXECUTABLE_SUFFIX ".Mach-O")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ESP_PLATFORM)
  set(EXECUTABLE_SUFFIX ".elf")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(EXECUTABLE_SUFFIX ".exe")
endif()

if ($ENV{FND_RELEASE_BUILD})
  message(STATUS "Building Release build")
  target_compile_options(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PRIVATE
    -ffunction-sections
    -fno-exceptions
    -fdata-sections
    -mlongcalls
    -Os
    -Wextra
    -std=gnu++23
  )

  target_compile_definitions(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC CONFIG_LOG_COLORS=0)

elseif ($ENV{FND_DEBUG_BUILD})
  message(STATUS "Building Debug build")
  target_compile_options(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PRIVATE
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -mlongcalls
    -O0
    -g3
    -ggdb
    -Wextra
    -std=gnu++23
  )

  target_compile_definitions(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PULBIC CONFIG_LOG_COLORS=1)

#default to debug build
else ()
  message(STATUS "Building debug build")
  target_compile_options(${PROJECT_NAME}${EXECUTABLE_SUFFIX}
  PRIVATE
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -mlongcalls
    -O0
    -g3
    -ggdb
    -Wextra
    -std=gnu++23
  )

  target_compile_definitions(${PROJECT_NAME}${EXECUTABLE_SUFFIX} PUBLIC CONFIG_LOG_COLORS=1)

endif()

add_subdirectory(main/AbstractionLayer)
target_include_directories(__idf_main PRIVATE $<TARGET_PROPERTY:abstractionLayer,INTERFACE_INCLUDE_DIRECTORIES>)

##Application layer. Cross-platform implementation core code
add_subdirectory(main/Foundation/Cleon)
add_subdirectory(main/Foundation/Clients/Cloud)
add_subdirectory(main/Foundation/Cloud)
add_subdirectory(main/Foundation/Factory)
add_subdirectory(main/Foundation/Main)
add_subdirectory(main/Foundation/Servers/Webapp)
add_subdirectory(main/Common)

##Module/Porting layer
##Modules are chunks of code that implement functionality. They are optional and can be removed.
##They also serve as a method of porting to Foundation on different platforms.
##E.g. to run on Linux, replace or add main/Modules/Linux/Drivers subdirectory.
##To remove modules, use the "None" port since at least a function stub must exist for Foundation
##core code to call.
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Uart/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Watchdog/Kevin)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Gpio/Esp)

add_subdirectory(main/AbstractionLayer/Modules/OperatingSystem/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Network/Wifi/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Ip/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Logging/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Storage/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Serialization/ClearBlueCloudProtobuf)
add_subdirectory(main/AbstractionLayer/Modules/Error/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Tools/Any/Crc)
add_subdirectory(main/AbstractionLayer/Modules/MemoryManagement/Esp)
add_subdirectory(main/AbstractionLayer/Modules/Cryptography/Esp)
add_subdirectory(main/AbstractionLayer/Utilities)
add_subdirectory(main/AbstractionLayer/Applications/Logging)
add_subdirectory(main/AbstractionLayer/Applications/ChainOfResponsibility)
