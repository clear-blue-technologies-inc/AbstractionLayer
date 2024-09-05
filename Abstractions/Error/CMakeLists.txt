target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  Error.hpp
)

add_library(Error INTERFACE)

target_include_directories(Error INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})