#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

namespace Logger
{
    // ------------------------------------------------------------
    // Log severity levels
    // ------------------------------------------------------------
    enum class Level
    {
        Info,
        Warning,
        Error,
        Debug
    };

    // ------------------------------------------------------------
    // Configure logger settings (optional)
    // Example:
    //   Logger::setLogFile("logs/output.txt");
    //   Logger::enableConsole(false);
    // ------------------------------------------------------------
    void setLogFile(const std::string& filepath);
    void enableConsole(bool state);
    void setLogLevel(Level level);

    // ------------------------------------------------------------
    // Core logging function
    // ------------------------------------------------------------
    void log(Level level, const std::string& message);

    // ------------------------------------------------------------
    // Convenience wrappers for common usage
    // ------------------------------------------------------------
    inline void info(const std::string& msg)
    {
        log(Level::Info, msg);
    }

    inline void warn(const std::string& msg)
    {
        log(Level::Warning, msg);
    }

    inline void error(const std::string& msg)
    {
        log(Level::Error, msg);
    }

    inline void debug(const std::string& msg)
    {
        log(Level::Debug, msg);
    }
}
