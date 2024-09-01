################################################################################
#Date: September 26th, 2023                                                    #
#File: CMakeLists.txt                                                          #
#Authour: Ben Haubrich                                                         #
#Synopsis: Top level CMakeList.txt.                                     #
################################################################################
cmake_minimum_required(VERSION 3.5)

#Use cmake '--help-policy <POLICY>` to learn more
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0076 NEW)

add_library(abstractionLayer INTERFACE)

add_subdirectory(Cryptography)
add_subdirectory(Error)
add_subdirectory(Event)
add_subdirectory(Hardware)
add_subdirectory(Ip)
add_subdirectory(Logging)
add_subdirectory(Network)
add_subdirectory(OperatingSystem)
add_subdirectory(Protocols)
add_subdirectory(Serialization)
add_subdirectory(Storage)
add_subdirectory(Types)
