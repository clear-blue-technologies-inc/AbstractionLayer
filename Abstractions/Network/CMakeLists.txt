target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  NetworkAbstraction.hpp
)

add_library(Network INTERFACE)

target_link_libraries(Network INTERFACE Event)
target_link_libraries(Network INTERFACE Utilities)
target_include_directories(Network INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})