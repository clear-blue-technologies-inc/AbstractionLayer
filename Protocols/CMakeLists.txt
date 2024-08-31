target_sources(abstractionLayer
PRIVATE FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
  CbtCommunicationProtocol.hpp
  CbtIcCommunicationProtocol.hpp
)
