#include <iostream>
#include "Logger.h"

using namespace kub;

int main()
{
    std::cout << "Kub C++ logger example:" << endl;

    LOGGER_SETTINGS.level = kub::Logger::Severity::verbose;
    LOGGER_SETTINGS.trueValue = "ON";
    LOGGER_SETTINGS.falseValue = "OFF";
    LOGGER_FILESINK(true,"log.txt");

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
