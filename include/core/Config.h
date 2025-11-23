#pragma once

#include <string>
#include <unordered_map>

class Config
{
public:
    Config() = default;
    ~Config() = default;

    // Load a .ini or .cfg style configuration file
    bool loadFromFile(const std::string& filepath);

    // Getters for configuration values
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int         getInt(const std::string& key, int defaultValue = 0) const;
    float       getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool        getBool(const std::string& key, bool defaultValue = false) const;

private:
    void parseLine(const std::string& line);

private:
    std::unordered_map<std::string, std::string> m_values;
};
