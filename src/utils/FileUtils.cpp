#include "FileUtils.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

namespace FileUtils
{
    // ------------------------------------------------------------
    // Check if file exists
    // ------------------------------------------------------------
    bool exists(const std::string& path)
    {
        return fs::exists(path);
    }

    // ------------------------------------------------------------
    // Read file as string
    // ------------------------------------------------------------
    std::string readFileAsString(const std::string& path)
    {
        if (!exists(path))
            return "";

        std::ifstream file(path);
        if (!file.is_open())
            return "";

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    // ------------------------------------------------------------
    // Read binary file
    // ------------------------------------------------------------
    bool readFileAsBinary(const std::string& path,
                          std::vector<unsigned char>& outData)
    {
        if (!exists(path))
            return false;

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            return false;

        file.unsetf(std::ios::skipws);

        std::streampos size;
        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        outData.reserve(size);
        outData.insert(outData.begin(),
                       std::istream_iterator<unsigned char>(file),
                       std::istream_iterator<unsigned char>());

        return true;
    }

    // ------------------------------------------------------------
    // List all files (non-recursive)
    // ------------------------------------------------------------
    std::vector<std::string> listFiles(const std::string& directory)
    {
        std::vector<std::string> files;

        if (!fs::exists(directory))
            return files;

        for (const auto& entry : fs::directory_iterator(directory))
        {
            if (entry.is_regular_file())
                files.push_back(entry.path().string());
        }

        std::sort(files.begin(), files.end());
        return files;
    }

    // ------------------------------------------------------------
    // Join paths cleanly
    // ------------------------------------------------------------
    std::string joinPath(const std::string& base,
                         const std::string& file)
    {
        fs::path p(base);
        p /= file;
        return p.string();
    }

    // ------------------------------------------------------------
    // Extract filename
    // ------------------------------------------------------------
    std::string getFilename(const std::string& path)
    {
        return fs::path(path).filename().string();
    }

    // ------------------------------------------------------------
    // Extract extension (lowercase)
    // ------------------------------------------------------------
    std::string getExtension(const std::string& path)
    {
        std::string ext = fs::path(path).extension().string();

        if (!ext.empty() && ext[0] == '.')
            ext.erase(ext.begin());

        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
}
