#include "Logger.h"

kub::Logger::Logger() {
	auto logHeader = []() {
		COLOR_LOG_WARNING;
		cout << "Logger";
		COLOR_LOG_DEFAULT;
		cout << " - The C++ logger utility" << endl;
		cout << "version 1.0.0" << endl;
		cout << "Copyright 2023 ";
		COLOR_LOG_GREEN;
		cout << "Kubware";
		COLOR_LOG_DEFAULT;
		cout << endl;
		cout << endl;
		};

	// Log header to standard output.
	logHeader();

}

kub::Logger::~Logger() {
	cout << "Logger END" << endl;
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

string kub::Logger::logStart(Severity& severity)
{
	string timePart = getCurrentTime();
	string severenityPart = getSeverentityCode(severity);

	if (Settings.consoleSink) {
	    COLOR_LOG_DEFAULT;
		cout << timePart << " |";
		switch (severity)
		{
		case Severity::fatal: COLOR_LOG_FATAL; break;
		case Severity::error:  COLOR_LOG_ERROR; break;
		case Severity::warning:COLOR_LOG_WARNING; break;
		case Severity::info:  COLOR_LOG_INFO; break;
		case Severity::debug: COLOR_LOG_DEBUG; break;
		case Severity::verbose: COLOR_LOG_VERBOSE; break;
		default: COLOR_LOG_DEFAULT; break;
		}
		cout << severenityPart;
		COLOR_LOG_DEFAULT;
		cout << "| ";
	}

	return string(timePart + " |" + severenityPart + "| ");
}
