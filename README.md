# AbstractionLayer
An abstraction layer for building cross-platform, component-based C++/CMake projects.

Include it in your CMake project using:

`add_subdirectory(AbstractionLayer)`

Pass the headers along to other libraries:

`target_link_libraries(<target>, <PUBLIC|PRIVATE>, abstractionLayer)`

Alternatively, you can inlcude only a portion of it, e.g. Uart:

`target_link_libraries(<target>, <PUBLIC|PRIVATE>, Uart)`

## Why use an abstraction layer
An abstraction layer allows you to build your main application code separate from common software components such as hardware peripherals, storage, networks, etc.
This allows you to swap out components without having to edit or retest your main application. Consequentially, this allows you to develop and test applications on systems such
as MacOS or Linux which have faster processing times and a more feature rich debugging environment, and then take that tested code and run it on an embedded target with your ported abstraction layer.

As an example, suppose I want to test the Hello World application:

```
//Ported code.
LoggingAbstraction *logger = new MacOSLogger();

//Main application code.
logger->log("LogTag", "Hello World", LogTpye::Info);
```

After testing on MacOS, my application works. So I bring it over to my embedded target:
```
//Ported code
LoggingAbstraction *logger = new EmbeddedLogger();

//Main application code.
logger->log("LogTag", "Hello World", LogTpye::Info);
```

The main application code hasn't been altered so I know the logic is correct. It will print as long as the ported code works. If it doesn't print I can eliminate 50% of the code (the main application code) as a potential source of the bug since it's already been tested and has not changed.

## How to include Abstraction Layer in your project
Your top-level `CMakeLists.txt` from your main application may include the top-level `CMakeLists.txt` of the abstraction layer as shown below:

```
if (ESP_PLATFORM)
  #If not set, defaults to all components. Set this to reduce the amount of
  #components that are built at compile time. Required and linked components
  #for foundation are located in the main component CMakeLists.txt file. When a
  #component is required, it's added to the components list automatically.
  set(COMPONENTS
    main
  )
  #include directive must come after the set(COMPONENTS*) directive
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)
  project(projectName)
  include(main/AbstractionLayer/esp.cmake)
else()
  project(projectName)
  include(main/AbstractionLayer/desktop.cmake)
endif()
```

Additional platforms can be added in this pattern.

Then you should add your main application files to  you top-level main application `CMakeLists.txt`

```
add_subdirectory(mainApplicationDir1)
add_subdirectory(mainApplicationDir2)
add_subdirectory(mainApplicationDir3)
...

You can do it before or after the inlcude. It doesn't matter.
