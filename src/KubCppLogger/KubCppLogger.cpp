// KubCppLogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example!\n";
       
    Logger::debug("Test");

    Logger::debug();   
}
