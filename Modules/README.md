Most modules begin by breaking down the varying ports by platform. In this way, the binary is compiled solely for the platform.

Other modules like Drivers and Network begin by breaking down the type of driver rather than the platform. This is because all the modules within
are fundamentally drivers or networks, but the implementation details are unique such that they can not be represented by a single abstraction.
For example, while setting the SSID on a wifi network is required for initialization, a cellular network instead uses an Access Point Name (APN).
If you attempted to make a "NetworkModule" that runs on different platforms, the modules would have to contain a lot of branches to handle varying types
of networks which would make the code messy, hard to read, hard to maintain and probably as a result also buggy.

When modules do not inherit the same base abstraction it allows for these modules to exist at the same time within the same binary and requires the use of Factories to get the right type of Driver or Network.

Most of the other modules, however, inherit from the same base abstraction so choosing them dynamically is not an option. They are chosen at compile time based on the platform.

If it makes sense for a module that can have different types that can potentially run at the same time then break them down by type first and then by platform. If they can't exist
at the same time then break them down by platform only.

A good example is the OperatingSystem. While there are many different types of OperatingSystems, it doesn't make sense to run a Posix OperatingSystem in the same binary as a FreeRTOS OperatingSystem. So to save some typing in your code base, you can just have a single OperatingSystem class that changes based on what platform you compile with.