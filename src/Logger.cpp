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
