#ifndef KUB_LOGGER_H
#define KUB_LOGGER_H

#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;
namespace kub {
   
    /// <summary>
    /// Kub C++ Logger main class.
    /// </summary>
    class Logger
    {
    public:        

        static void debug(string message = "No data") {
            cout << message << endl;
        }

    private:

        /// <summary>
        /// Gets the singleton instance for the logger with settings.
        /// </summary>
        /// <returns></returns>
        static Logger& getInstance()
        {
            static Logger instance; // Guaranteed to be destroyed.
            // Instantiated on first use.           
            return instance;
        }

        /// <summary>
        /// Hidden constructor.
        /// </summary>
        Logger() {
            cout << "Start" << endl;
            level = 1;
        }                   

        /// <summary>
        /// Hidden destructor.
        /// </summary>
        ~Logger() {
            cout << "END" << endl;
        }
         
        /// <summary>
        /// Logging level.
        /// </summary>
        int level;     

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