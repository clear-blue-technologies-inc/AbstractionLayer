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
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Uart/MacOsCleonSimulator)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Watchdog/None)
add_subdirectory(main/AbstractionLayer/Modules/Drivers/Gpio/None)

add_subdirectory(main/AbstractionLayer/Modules/OperatingSystem/Posix)
add_subdirectory(main/AbstractionLayer/Modules/Network/Wifi/MacOs)
add_subdirectory(main/AbstractionLayer/Modules/Ip/Posix)
add_subdirectory(main/AbstractionLayer/Modules/Logging/Stdlib)
add_subdirectory(main/AbstractionLayer/Modules/Storage/Stdlib)
add_subdirectory(main/AbstractionLayer/Modules/Serialization/ClearBlueCloudProtobuf)
add_subdirectory(main/AbstractionLayer/Modules/Error/Errno)
add_subdirectory(main/AbstractionLayer/Modules/Tools/Any/Crc)
add_subdirectory(main/AbstractionLayer/Modules/MemoryManagement/Default)
add_subdirectory(main/AbstractionLayer/Modules/Cryptography/MacOs)
add_subdirectory(main/AbstractionLayer/Utilities)
add_subdirectory(main/AbstractionLayer/Applications/Logging)
add_subdirectory(main/AbstractionLayer/Applications/ChainOfResponsibility)
add_subdirectory(main/AbstractionLayer/Applications/Event)