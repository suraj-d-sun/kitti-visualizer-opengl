#include "Config.h"
#include "FileUtils.h"
#include "Logger.h"

#include <sstream>
#include <algorithm>

Config::Config(const std::string& filepath)
    : m_filepath(filepath)
{
}

bool Config::load()
{
    Logger::info("Loading config: " + m_filepath);

    std::string content = FileUtils::readFileAsString(m_filepath);
    if (content.empty())
    {
        Logger::warn("Config file is empty or not found: " + m_filepath);
        return false;
    }

    std::istringstream ss(content);
    std::string line;

    while (std::getline(ss, line))
    {
        // Remove spaces
        line.erase(std::remove_if(line.begin(), line.end(),
                     [](unsigned char c) { return c == ' ' || c == '\t'; }),
                   line.end());

        // Skip empty or comment lines
        if (line.empty() || line[0] == '#')
            continue;

        auto delimPos = line.find('=');
        if (delimPos == std::string::npos)
            continue;

        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);

        m_values[key] = value;
    }

    Logger::info("Config loaded successfully.");
    return true;
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const
{
    auto it = m_values.find(key);
    return (it != m_values.end()) ? it->second : defaultValue;
}

int Config::getInt(const std::string& key, int defaultValue) const
{
    auto it = m_values.find(key);
    if (it != m_values.end())
    {
        try {
            return std::stoi(it->second);
        }
        catch (...)
        {
            Logger::warn("Failed to parse int for key: " + key);
        }
    }
    return defaultValue;
}

float Config::getFloat(const std::string& key, float defaultValue) const
{
    auto it = m_values.find(key);
    if (it != m_values.end())
    {
        try {
            return std::stof(it->second);
        }
        catch (...)
        {
            Logger::warn("Failed to parse float for key: " + key);
        }
    }
    return defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) const
{
    auto it = m_values.find(key);
    if (it != m_values.end())
    {
        std::string val = it->second;
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);

        if (val == "true" || val == "1" || val == "yes")
            return true;
        if (val == "false" || val == "0" || val == "no")
            return false;

        Logger::warn("Failed to parse bool for key: " + key);
    }
    return defaultValue;
}

bool Config::has(const std::string& key) const
{
    return m_values.find(key) != m_values.end();
}
