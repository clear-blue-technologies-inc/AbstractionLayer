################################################################################
#Date: September 26th, 2023                                                    #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level project CMakeList.txt for AbstractionLayer desktop build  #
################################################################################
add_subdirectory(${CMAKE_CURRENT_LIST_DIR})

#Module/Porting layer
#Modules are chunks of code that implement functionality. They are all interchangeable provided that the target platform
#supports the implementation.
#They serve as a method of porting to Foundation on different platforms.
#E.g. to run on Linux, replace or add main/Modules/Drivers/Linux/<module> subdirectory.
#To remove modules, use the "None" port since at least a function stub must exist for Foundation
#core code to call.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Uart/None)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Watchdog/None)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Gpio/None)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/OperatingSystem/Posix)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Network/Wifi/MacOs)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Ip/Posix)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Logging/Stdlib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Storage/Stdlib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Serialization/None)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Error/Errno)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Tools/Any/Crc)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/MemoryManagement/Default)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Cryptography/MacOs)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/Logging)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/ChainOfResponsibility)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/Event)