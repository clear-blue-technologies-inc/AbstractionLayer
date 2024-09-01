# AbstractionLayer
An abstraction layer for building cross-platform, component-based C++/CMake projects.

Include it in your CMake project using:
`add_subdirectory(AbstractionLayer)`
`target_link_libraries(<target>, <PUBLIC|PRIVATE>, abstractionLayer)`
Alternatively, you can inlcude only a portion of it, e.g. Uart:
`target_link_libraries(<target>, <PUBLIC|PRIVATE>, Uart)`
