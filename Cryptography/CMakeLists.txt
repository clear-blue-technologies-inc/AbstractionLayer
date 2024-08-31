target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CryptographyAbstraction.hpp
)

add_library(Cryptography INTERFACE)

target_include_directories(Cryptography INTERFACE ${CMAKE_CURRENT_LIST_DIR})
