target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  Types.hpp
)

add_library(Types INTERFACE)

target_include_directories(Types INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})
