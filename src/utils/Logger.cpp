#include "Logger.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

namespace Logger
{
    // ------------------------------------------------------------
    // Internal state
    // ------------------------------------------------------------
    static std::ofstream g_logFile;
    static bool g_consoleEnabled = true;
    static Level g_logLevel = Level::Info;

    static std::mutex g_mutex;

    // ------------------------------------------------------------
    // Helper: Convert enum Level → string
    // ------------------------------------------------------------
    static const char* levelToString(Level lvl)
    {
        switch (lvl)
        {
            case Level::Info:    return "INFO";
            case Level::Warning: return "WARN";
            case Level::Error:   return "ERROR";
            case Level::Debug:   return "DEBUG";
        }
        return "LOG";
    }

    // ------------------------------------------------------------
    // Helper: timestamp [YYYY-MM-DD HH:MM:SS]
    // ------------------------------------------------------------
    static std::string timestamp()
    {
        using clock = std::chrono::system_clock;
        auto now = clock::now();
        std::time_t t = clock::to_time_t(now);

        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // ------------------------------------------------------------
    // Configure log file output
    // ------------------------------------------------------------
    void setLogFile(const std::string& filepath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);

        if (g_logFile.is_open())
            g_logFile.close();

        g_logFile.open(filepath, std::ios::out | std::ios::app);

        if (!g_logFile.is_open())
        {
            std::cerr << "[Logger] Failed to open log file: "
                      << filepath << std::endl;
        }
    }

    // ------------------------------------------------------------
    // Enable/Disable console output
    // ------------------------------------------------------------
    void enableConsole(bool state)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_consoleEnabled = state;
    }

    // ------------------------------------------------------------
    // Set minimum log level
    // ------------------------------------------------------------
    void setLogLevel(Level level)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_logLevel = level;
    }

    // ------------------------------------------------------------
    // Main logging function
    // ------------------------------------------------------------
    void log(Level lvl, const std::string& message)
    {
        if (lvl < g_logLevel)
            return; // filtered out
        
        std::lock_guard<std::mutex> lock(g_mutex);

        std::string finalMessage =
            "[" + timestamp() + "] [" + levelToString(lvl) + "] " + message;

        // Console logging
        if (g_consoleEnabled)
        {
            switch (lvl)
            {
            case Level::Info:    std::cout  << finalMessage << std::endl; break;
            case Level::Warning: std::cout  << finalMessage << std::endl; break;
            case Level::Error:   std::cerr << finalMessage << std::endl; break;
            case Level::Debug:   std::cout  << finalMessage << std::endl; break;
            }
        }

        // File logging
        if (g_logFile.is_open())
        {
            g_logFile << finalMessage << std::endl;
            g_logFile.flush();
        }
    }
}
