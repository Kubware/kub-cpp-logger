# Kubware C++ Logger

**WARNING - UNDER DEVELOPMENT!!! - do not use nuget pack nuget/KubCppLogger.nuspec -outputDirectory C:/NuGet -verbosity detailed**

**Kubware C++ Logger** is simple console or file logger with basic 6 levels of importance:

 - VERBOSE
 - DEBUG
 - INFO
 - WARNING
 - ERROR
 - FATAL

## Examples

Link *Logger.h* header file and **kub** namespace.

```c++
#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example:" << endl;

    LOGGER_SETTINGS.level = kub::Logger::Severity::verbose;
    LOGGER_SETTINGS.trueValue = "ON";
    LOGGER_SETTINGS.falseValue = "OFF";

    LOG_VERBOSE("Verbose log");
    LOG_DEBUG("Debug log");
    LOG_INFO("Info log");
    LOG_WARNING("Warning log");
    LOG_ERROR("Error log");
    LOG_FATAL("Fatal log");
    LOG_INFO("");    
    LOG_INFO("String argument: {0}", "Hello");
    LOG_INFO("Numbers arguments: int: {0} / short: {0}, float: {0} / double: {0} ", 1000, (short)255, 0.12f, 0.13);
    LOG_INFO("Boolean argument: true: {0} / false: {0}", true, false);   
}
```

## Using and installing

There are two files containing the logger:

 - **Logger.h**
 - **Logger.cpp**


### Manual copy

You can simply include the **Logger.cpp** and **KubLogger.h** files in your project and build together with your other source code.

### Include using NuGet package

To build NuGet package, install **NuGet.CommandLine** package first.
Than run next command in solution folder using *Package Manager Console*:

```verbatim
nuget pack nuget/KubCppLogger.nuspec -outputDirectory C:/NuGet -verbosity detailed 
```

It will create **NuGet** package in C:/NuGet folder, that can be referenced by *NuGet Package manager* as local source.

## Licence

**KubLogger** is available under [MIT-licence](./LICENSE.md).

&copy; Kubware 2023


