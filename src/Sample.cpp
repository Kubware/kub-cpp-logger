// KubCppLogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example!\n";
       
    Logger::getLoggerInstance().logMessage(LOG_LEVEL_ERROR,"Test");

    LOGGER.debug("Test");

    Logger::debug("x");

    LOG_DEBUG("Debug");

    LOGGER.logIt(LOG_LEVEL_INFO, "{0} world{0} {0} {0} ss\n", "Hello", 0.01, 123, true);
   
    
    LOGGER.logIt(LOG_LEVEL_INFO, "{0} world{0} {110\n", "Hello", '!', 123);

    
}
