# AbstractionLayer
An abstraction layer for building cross-platform, component-based C++/CMake projects.

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

Then you should add your main application files to your top-level main application `CMakeLists.txt`

```
add_subdirectory(mainApplicationDir1)
add_subdirectory(mainApplicationDir2)
add_subdirectory(mainApplicationDir3)
```

You can do it before or after the inlcude. It doesn't matter.

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

The main application code hasn't changed so I know the logic is correct. It will print as long as the ported code works. If it doesn't print I can eliminate the main application code as a potential source of the bug since it's already been tested and has not changed.

## Reduce dependancies on chip vendor build tools
This abstraction layer allows easy changes away from drivers and build tools offered by your chip vendor. If you mix in vendor specific tools and drivers directly alongside your application, the application will develop a strong dependancy on it as it's size and complexity increases. Some of these dependancies are:
- return types
- parameters
- function name
  
It will be difficult to port software like this to other platforms. Never say never. Anything can happen in a project with a long or even indefinite lifetime including having/wanting to support another processor or switch vendors. If you don't pay 
attention to how your application manages dependancies even a smaller hardware change like a bluetooth chip, RTC, ADC, network switch can surprise you with how embedded it's become in your applications correctness.
If you depend on an abstraction:
- the return type almost never changes.
- the parameters almost never change.
- the function name almost never changes.
  I say almost because it can be challenging to develop an abstraction layer that can support every driver on the market, but _almost never change_ is better than _certainly will change_.
