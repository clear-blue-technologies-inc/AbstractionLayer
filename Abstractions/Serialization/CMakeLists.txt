target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  SerializationAbstraction.hpp
)

add_library(Serialization INTERFACE)

target_include_directories(Serialization INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(abstractionLayer INTERFACE ${CMAKE_CURRENT_LIST_DIR})