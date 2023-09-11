# Kubware C++ Logger

**Kubware C++ Logger** is simple console or file logger with basic 6 levels of importance:

 - VERBOSE
 - DEBUG
 - INFO
 - WARNING
 - ERROR
 - FAIL

## Examples

Link *Logger.h* header file and **kub** namespace.

```c++
#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example!\n";       
    Logger::debug("Test");
    Logger::debug();   
}
```

## Using and installing

There are two files containing the logger:

 - **KubLogger.cpp**
 - **KubLogger.h**

You simply include the KubLogger.cpp and KubLogger.h files in your project and build with your other source code.

The solution is used for development purposes.

## Licence

**KubLogger** is available under [MIT-licence](./LICENSE.md).

&copy; Kubware 2023


