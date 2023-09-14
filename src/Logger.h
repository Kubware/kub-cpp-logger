#ifndef KUB_LOGGER_H
#define KUB_LOGGER_H

#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#   include <time.h>
#   include <windows.h>

#define COLOR_LOG_DEFAULT		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#define COLOR_LOG_GREEN		    ""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
#define COLOR_LOG_FATAL			""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 207);
#define COLOR_LOG_ERROR			""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#define COLOR_LOG_WARNING		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
#define COLOR_LOG_INFO			""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#define COLOR_LOG_DEBUG		    ""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#define COLOR_LOG_VERBOSE		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
#define COLOR_LOG_MEMORY		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
#define COLOR_LOG_NUMBER		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
#define COLOR_LOG_DETAIL		""; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

#else
#   include <unistd.h>
#   include <sys/time.h>
#   if defined(__linux__) || defined(__FreeBSD__)
#       include <sys/syscall.h>
#   elif defined(__rtems__)
#       include <rtems.h>
#   endif
#   if defined(_POSIX_THREADS)
#       include <pthread.h>
#   endif
#   if PLOG_ENABLE_WCHAR_INPUT
#       include <iconv.h>
#   endif
#endif

#ifdef _MSC_VER
#   define HEXLOGGER_GET_FUNC()      __FUNCTION__
#   define HEXLOGGER_GET_FILENAME() (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#elif defined(__BORLANDC__)
#   define HEXLOGGER_GET_FUNC()      __FUNC__
#   define HEXLOGGER_GET_FILENAME() (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#   define HEXLOGGER_GET_FUNC()      __PRETTY_FUNCTION__
#   define HEXLOGGER_GET_FILENAME() (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif


#define VERBOSE(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::verbose, message, __VA_ARGS__);
#define DEBUG(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::debug, message, __VA_ARGS__); 
#define INFO(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::info, message, __VA_ARGS__); 
#define WARNING(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::warning, message, __VA_ARGS__); 
#define ERROR(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::error, message, __VA_ARGS__); 
#define FATAL(message,...)         Logger::getLoggerInstance().logMessage(kub::Logger::Severity::fatal, message, __VA_ARGS__); 

using namespace std;
namespace kub {


	/// <summary>
	/// Kub C++ Logger main class.
	/// </summary>
	class Logger
	{
	public:

		/// <summary>
		/// Log severentity / level.
		/// </summary>
		enum class Severity : short
		{
			/// <summary>
			/// Do not log anything.
			/// </summary>
			none = 0,
			/// <summary>
			/// 
			/// </summary>
			fatal = 1,
			/// <summary>
			/// 
			/// </summary>
			error = 2,
			/// <summary>
			/// Warning
			/// </summary>
			warning = 3,
			/// <summary>
			/// Major information logs. To be shown in runtime.
			/// </summary>
			info = 4,
			/// <summary>
			/// 
			/// </summary>
			debug = 5,			
			/// <summary>
			/// Full detail logs.
			/// </summary>
			verbose = 6,

		};

		/// <summary>
		/// Gets the singleton instance for the logger with settings.
		/// </summary>
		/// <returns></returns>
		static Logger& getLoggerInstance()
		{
			static Logger instance; // Guaranteed to be destroyed.
			// Instantiated on first use.           
			return instance;
		}
				
		/// <summary>
		/// Set logger severenity level.
		/// </summary>
		/// <param name="level">New logger severenity level.</param>
		void setLevel(Severity level) {
			mMaxSeverity = level;
		}		

		template<typename... Targs>
		void logMessage(Severity severity, const char* format, Targs... Fargs) {

			// Ignore lower severenity
			if (severity > mMaxSeverity) return;

			// No sink - ignore log
			if (mConsoleSink == false && mFileSink == false) return;

			// Stream for file log
			std::stringstream ss;

			// Get time and severenity log start
			ss << logStart(severity);

			printArgument(ss, format, Fargs...); // recursive call

			if (mFileSink) {
				std::cout << ss.str() << endl;
			}
			return;

		}


		/// <summary>
		/// Base for recursive variadic function.
		/// </summary>
		/// <param name="ss">Stream for file log output.</param>
		/// <param name="format"></param>
		void printArgument(std::stringstream& ss, const char* format)
		{
			std::cout << format << endl;
		}

		/// <summary>
		/// Print next argument from formated message.
		/// </summary>
		/// <typeparam name="T">Current argument type.</typeparam>
		/// <typeparam name="...Targs">Arguments of recursive variadic function.</typeparam>
		/// <param name="ss">Stream for file log output.</param>
		/// <param name="format">Formated message with argument placeholders.</param>
		/// <param name="value">Current argument</param>
		/// <param name="...Fargs">Other arguments of recursive variadic function.</param>
		template<typename T, typename... Targs>
		void printArgument(std::stringstream& ss, const char* format, T value, Targs... Fargs)
		{
			for (; *format != '\0'; format++)
			{
				if (*format == '{')
				{
					format++;
					int len = 0;
					for (; *format != '}'; format++) {
						if (*format == '\0' || *format == '{') {
							if (mConsoleSink) {
								cout << COLOR_LOG_ERROR
									std::cout << " Log SYNTAX error!" << endl;
								cout << COLOR_LOG_DEFAULT
							}
							ss << " Log SYNTAX error!" << endl;
							return;
						}
						len++;
					}

					if (mConsoleSink) {

						if (std::is_same<T, int>::value) {
							cout << COLOR_LOG_WARNING
								std::cout << value;
						}
						else if (std::is_same<T, double>::value) {
							cout << COLOR_LOG_GREEN
								std::cout << value;
						}
						else if (std::is_same<T, bool>::value) {
							cout << COLOR_LOG_GREEN
								if ((bool)value == true) {
									std::cout << "ON";
								}
								else {
									std::cout << "OFF";
								}								
						}
						else {
							cout << COLOR_LOG_DETAIL
								std::cout << value;
						}

						
							
						cout << COLOR_LOG_DEFAULT
					}

					ss << value;
					printArgument(ss, format + len, Fargs...); // recursive call
					return;
				}
				std::cout << *format;
				ss << *format;
			}
		}

	private:

		/// <summary>
		/// Get current time.
		/// </summary>
		/// <returns></returns>
		static string getCurrentTime();

		/// <summary>
		/// Gets the severenity 3 character code.
		/// </summary>
		/// <param name="severity"></param>
		/// <returns></returns>
		static string getSeverentityCode(Severity& severity)
		{
			switch (severity)
			{
			case Severity::fatal:       return "FTL";
			case Severity::error:       return "ERR";
			case Severity::warning:     return "WRN";
			case Severity::info:        return "INF";
			case Severity::debug:       return "DBG";
			case Severity::verbose:     return "VRB";
			default:                    return "---";
			}
		}

		Severity mMaxSeverity;

		bool mConsoleSink;

		bool mFileSink;

	private:


		string logStart(Severity& severity);

		/// <summary>
		/// Hidden constructor.
		/// </summary>
		Logger(Severity maxSeverity = Severity::verbose);

		/// <summary>
		/// Hidden destructor.
		/// </summary>
		~Logger() {
			cout << "END" << endl;
		}

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name=""></param>
		Logger(Logger const&);              // Don't Implement

	public:

		void operator=(Logger const&) = delete;
	};



}


#endif /* KUB_LOGGER_H */