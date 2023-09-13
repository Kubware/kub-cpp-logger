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

   
    
}
