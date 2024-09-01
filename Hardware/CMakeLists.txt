target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  UartAbstraction.hpp
  GpioAbstraction.hpp
  WatchdogAbstraction.hpp
)

add_library(Uart INTERFACE)
add_library(Gpio INTERFACE)
add_library(Watchdog INTERFACE)

target_include_directories(Uart INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Watchdog INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})