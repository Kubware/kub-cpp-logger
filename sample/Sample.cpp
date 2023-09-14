// Sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example!\n";

    Logger::getLoggerInstance().logMessage(kub::Logger::Severity::info , "Test");
        
       

    VERBOSE("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);
    DEBUG("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);
    INFO("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);
    WARNING("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);
    ERROR("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);
    FATAL("{0} world{0} {0} {0} ss", "Hello", 0.01, 123, true);

}

