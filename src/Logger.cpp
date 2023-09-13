#include "Logger.h"

void kub::Logger::logMessage(Severity severity, string message)
{
    // Ignore lower severenity
    if (severity > mMaxSeverity) return;

    // Get time and severenity log start
    string start = logStart(severity);

    if (mConsoleSink) {
        cout << message << endl;
    }
}

void kub::Logger::debug(string message)
{
    this->logMessage(Logger::Severity::debug, message);
}

string kub::Logger::getCurrentTime()
{
    stringstream ss;
    struct tm newtime;
    __time64_t long_time;
    errno_t err;

    auto now = chrono::system_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    long_time = chrono::system_clock::to_time_t(now);
    _time64(&long_time);
    err = _localtime64_s(&newtime, &long_time);
    ss << put_time(&newtime, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

string kub::Logger::getSeverentityCode(Severity& severity)
{
    switch (severity)
    {
    case Severity::fatal:       return "FTL";
    case Severity::error:       return "ERR";
    case Severity::warning:     return "WRN";
    case Severity::info:        return "INF";
    case Severity::debug:       return "DBG";
    case Severity::memory:      return "MEM";
    case Severity::verbose:     return "VRB";
    default:                    return "---";
    }
}

string kub::Logger::logStart(Severity& severity)
{
    string timePart = getCurrentTime();
    string severenityPart = getSeverentityCode(severity);

    if (mConsoleSink) {
        cout << COLOR_LOG_DEFAULT;
        cout << timePart << " |";
        string color = "";
        switch (severity)
        {
        case Severity::fatal: color = COLOR_LOG_FATAL break;
        case Severity::error: color = COLOR_LOG_ERROR break;
        case Severity::warning: color = COLOR_LOG_WARNING break;
        case Severity::info: color = COLOR_LOG_INFO break;
        case Severity::debug:color = COLOR_LOG_DEBUG break;
        case Severity::verbose: color = COLOR_LOG_VERBOSE break;
        case Severity::memory: color = COLOR_LOG_MEMORY break;
        default: color = COLOR_LOG_DEFAULT break;
        }
        cout << color;
        cout << severenityPart;
        cout << COLOR_LOG_DEFAULT;
        cout << "| ";
    }

    return string(timePart + " |" + severenityPart + "| ");
}
