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

The main application code hasn't been altered so I know the logic is correct. It will print as long as the ported code works.
