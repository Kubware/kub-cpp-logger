#include "Logger.h"



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
		default: color = COLOR_LOG_DEFAULT break;
		}
		cout << color;
		cout << severenityPart;
		cout << COLOR_LOG_DEFAULT;
		cout << "| ";
	}

	return string(timePart + " |" + severenityPart + "| ");
}

/// <summary>
/// Hidden constructor.
/// </summary>

kub::Logger::Logger(Severity maxSeverity)
	: mMaxSeverity(maxSeverity), mConsoleSink(true), mFileSink(false) {
	cout << "Start" << endl;

	auto logHeader = []() {
		cout << COLOR_LOG_WARNING
			cout << "Logger";
		cout << COLOR_LOG_DEFAULT
			cout << " - The C++ logger utility" << endl;
		cout << "version 1.0.0" << endl;
		cout << "Copyright 2023 ";
		cout << COLOR_LOG_GREEN
			cout << "Kubware";
		cout << COLOR_LOG_DEFAULT
			cout << endl;
		cout << endl;
		};

	// Log header to standard output.
	logHeader();

}
