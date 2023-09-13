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

#define LOG_LEVEL_MEMORY        kub::Logger::Severity::memory
#define LOG_LEVEL_VERBOSE       kub::Logger::Severity::verbose
#define LOG_LEVEL_DEBUG         kub::Logger::Severity::debug
#define LOG_LEVEL_INFO          kub::Logger::Severity::info
#define LOG_LEVEL_WARNING       kub::Logger::Severity::warning
#define LOG_LEVEL_ERROR         kub::Logger::Severity::error
#define LOG_LEVEL_LOG_FATAL     kub::Logger::Severity::fatal
#define LOG_LEVEL_NONE          kub::Logger::Severity::none

#define LOGGER Logger::getLoggerInstance()

#define LOG_DEBUG(...)          LOGGER.logMessage(LOG_LEVEL_DEBUG,__VA_ARGS__) 

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
            /// Memory detail log.
            /// </summary>
            memory = 6,
            /// <summary>
            /// Full detail logs.
            /// </summary>
            verbose = 7,
            
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
            
        static void debug(string message);

        /// <summary>
        /// Set logger severenity level.
        /// </summary>
        /// <param name="level">New logger severenity level.</param>
        void setLevel(Severity level) {
            mMaxSeverity = level;
        }


        void logMessage(Severity severity, string message);


        template<typename T, typename... Targs>
        void logIt(Severity severity, const char* format, T value, Targs... Fargs) {

            // Ignore lower severenity
            if (severity > mMaxSeverity) return;

            // Get time and severenity log start
            string start = logStart(severity);

            if (mConsoleSink) {
                for (; *format != '\0'; format++)
                {
                    if (*format == '%')
                    {
                        std::cout << value;
                        tprintf(format + 1, Fargs...); // recursive call
                        return;
                    }
                    std::cout << *format;
                }
            }
        }


        void tprintf(const char* format) // base function
        {
            std::cout << format;
        }

        template<typename T, typename... Targs>
        void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
        {
            for (; *format != '\0'; format++)
            {
                if (*format == '%')
                {
                    std::cout << value;
                    tprintf(format + 1, Fargs...); // recursive call
                    return;
                }
                std::cout << *format;
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

        Severity mMaxSeverity;

        bool mConsoleSink;

        bool mFileSink;

    private:

        string logStart(Severity& severity);

        

        /// <summary>
        /// Hidden constructor.
        /// </summary>
        Logger(Severity maxSeverity = Severity::verbose) 
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