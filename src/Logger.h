/*
 * Copyright (c) 2023 Jakub Èerkala (https://github.com/KubCode)
 * 
 * MIT Licence 
 */
 

#ifndef KUB_LOGGER_H
#define KUB_LOGGER_H

#define LOGGER_VERSION "1.0.0"

#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#   include <time.h>
#   include <windows.h>

#ifndef KUB_LOGGER_COLOR_SCHEME
#define KUB_LOGGER_COLOR_SCHEME

#define COLOR_LOG_DEFAULT		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)
#define COLOR_LOG_GREEN		    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10)
#define COLOR_LOG_FATAL			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 207)
#define COLOR_LOG_ERROR			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12)
#define COLOR_LOG_WARNING		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14)
#define COLOR_LOG_INFO			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)
#define COLOR_LOG_DEBUG		    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)
#define COLOR_LOG_VERBOSE		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8)
#define COLOR_LOG_NUMBER		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13)

#define COLOR_LOG_STRING        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6)  // gold
#define COLOR_LOG_INTEGER       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3)  // light blue
#define COLOR_LOG_FLOAT         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13) // magenta
#define COLOR_LOG_BOOL_TRUE     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10) // green
#define COLOR_LOG_BOOL_FALSE	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12) // false

#endif // KUB_LOGGER_COLOR_SCHEME

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


#define LOG_VERBOSE(message,...)        Logger::getLoggerInstance().logMessage(kub::Logger::Severity::verbose, message, __VA_ARGS__)
#define LOG_DEBUG(message,...)			Logger::getLoggerInstance().logMessage(kub::Logger::Severity::debug, message, __VA_ARGS__)
#define LOG_INFO(message,...)			Logger::getLoggerInstance().logMessage(kub::Logger::Severity::info, message, __VA_ARGS__) 
#define LOG_WARNING(message,...)        Logger::getLoggerInstance().logMessage(kub::Logger::Severity::warning, message, __VA_ARGS__)
#define LOG_ERROR(message,...)			Logger::getLoggerInstance().logMessage(kub::Logger::Severity::error, message, __VA_ARGS__)
#define LOG_FATAL(message,...)			Logger::getLoggerInstance().logMessage(kub::Logger::Severity::fatal, message, __VA_ARGS__)

#define LOGGER_SETTINGS					Logger::getLoggerInstance().Settings
#define LOGGER_FILESINK(enabled,...)	Logger::getLoggerInstance().setFileSink(enabled, __VA_ARGS__)

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
		/// Current logger settings.
		/// </summary>
		struct LoggerSettings {
			/// <summary>
			/// Logging minimal severenity included.
			/// </summary>
			Severity level = kub::Logger::Severity::verbose;
			/// <summary>
			/// If true, output to console.
			/// </summary>
			bool consoleSink = true;			
			/// <summary>
			/// Value printed if boolean argument is true.
			/// </summary>
			string trueValue = "true";
			/// <summary>
			/// Value printed if boolean argument is false.
			/// </summary>
			string falseValue = "false";			
		}
		Settings;

	public:

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
		/// Log message with specific severenity and arguments.
		/// </summary>
		/// <typeparam name="...Targs">Arguments of recursive variadic function.</typeparam>
		/// <param name="severity">Mesage severenitz.</param>
		/// <param name="format">Message in required format.</param>
		/// <param name="...Fargs"></param>
		template<typename... Targs>
		void logMessage(Severity severity, const char* format, Targs... Fargs) {

			// Ignore lower severenity
			if (severity > Settings.level) return;

			// No sink - ignore log
			if (Settings.consoleSink == false && mFilesink == false) return;

			// Stream for file log
			std::stringstream ss;

			// Get time and severenity log start
			ss << logStart(severity);

			// Recursive argument printing
			printArgument(ss, format, Fargs...);

			// Append to file
			if (mFilesink) {
				ofstream logFile;
				logFile.open(mFilePath, ios::out | ios::app);
				logFile << ss.str();
				logFile.close();
			}
			return;
		}

		/// <summary>
		/// Set logging to file.
		/// </summary>
		/// <param name="enabled">If true, logs will be sinked to file.</param>
		/// <param name="filepath">Filepath to logs file with an extension.</param>
		/// <returns>True, if file created successfuly. Otherwise false.</returns>
		bool setFileSink(bool enabled, string filepath = "log.txt");

		/// <summary>
		/// Base for recursive variadic function.
		/// </summary>
		/// <param name="ss">Stream for file log output.</param>
		/// <param name="format">Message in required format.</param>
		void printArgument(std::stringstream& ss, const char* format)
		{
			if (Settings.consoleSink) {
				std::cout << format << endl;
			}			

			if (mFilesink) {
				ss << format << endl;
			}
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
							if (Settings.consoleSink) {
								COLOR_LOG_ERROR;
								cout << " Log SYNTAX error!" << endl;
								COLOR_LOG_DEFAULT;
							}
							ss << " Log SYNTAX error!" << endl;
							return;
						}
						len++;
					}

					if (Settings.consoleSink) {
						if (std::is_same<T, int>::value || std::is_same<T, short>::value) {
							COLOR_LOG_INTEGER;
							std::cout << value;
						}
						else if (std::is_same<T, double>::value || std::is_same<T, float>::value) {
							COLOR_LOG_FLOAT;
							std::cout << value;
						}
						else if (std::is_same<T, bool>::value) {
							if ((bool)value == true) {
								COLOR_LOG_BOOL_TRUE;
								std::cout << Settings.trueValue;
							}
							else {
								COLOR_LOG_BOOL_FALSE;
								std::cout << Settings.falseValue;
							}
						}
						else {
							COLOR_LOG_STRING;
							std::cout << value;
						}
						COLOR_LOG_DEFAULT;
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
		static string getSeverentityCode(Severity& severity);

	private:

		/// <summary>
		/// Log time and severenity code.
		/// </summary>
		/// <param name="severity">Log message severenity.</param>
		/// <returns></returns>
		string logStart(Severity& severity);

		/// <summary>
		/// Hidden constructor.
		/// </summary>
		Logger();

		/// <summary>
		/// Hidden destructor.
		/// </summary>
		~Logger();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name=""></param>
		Logger(Logger const&);              // Don't Implement

		/// <summary>
		/// If true, logs are sinked to file.
		/// </summary>
		bool mFilesink = false;

		/// <summary>
		/// Path to file to log.
		/// </summary>
		string mFilePath = "log.txt";

	public:
		void operator=(Logger const&) = delete;
	};
}


#endif /* KUB_LOGGER_H */