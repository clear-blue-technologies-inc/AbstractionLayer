################################################################################
#Date: April 17th, 2023                                                        #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level project CMakeList.txt for AbstractionLayer ESP build      #
################################################################################
add_subdirectory(${CMAKE_CURRENT_LIST_DIR})

#Module/Porting layer
#Modules are chunks of code that implement functionality. They are all interchangeable provided that the target platform
#supports the implementation.
#They serve as a method of porting to Foundation on different platforms.
#E.g. to run on Linux, replace or add main/Modules/Drivers/Linux/<module> subdirectory.
#To remove modules, use the "None" port since at least a function stub must exist for Foundation
#core code to call.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Uart/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Watchdog/Kevin)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Drivers/Gpio/Esp)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/OperatingSystem/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Network/Wifi/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Ip/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Logging/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Storage/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Serialization/ClearBlueCloudProtobuf)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Error/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Tools/Any/Crc)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/MemoryManagement/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Modules/Cryptography/Esp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/Logging)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/ChainOfResponsibility)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Applications/Event)
