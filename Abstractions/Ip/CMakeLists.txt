target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  IpClientAbstraction.hpp
  IpServerAbstraction.hpp
)

add_library(IpClient INTERFACE)
add_library(IpServer INTERFACE)

target_include_directories(IpClient INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(IpServer INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})
