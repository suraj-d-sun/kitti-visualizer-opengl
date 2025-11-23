#pragma once

#include <string>
#include <vector>

namespace FileUtils
{
    // ------------------------------------------------------------
    // Check if a file exists on disk
    // ------------------------------------------------------------
    bool exists(const std::string& path);

    // ------------------------------------------------------------
    // Read an entire file as a string
    // For text files (e.g., shader sources, config files)
    // ------------------------------------------------------------
    std::string readFileAsString(const std::string& path);

    // ------------------------------------------------------------
    // Read binary file into a byte array
    // For LiDAR .bin files, image buffers, etc.
    // ------------------------------------------------------------
    bool readFileAsBinary(const std::string& path,
                          std::vector<unsigned char>& outData);

    // ------------------------------------------------------------
    // List directory contents (files only, not recursive)
    // Example: list all frames in a KITTI sequence folder
    // ------------------------------------------------------------
    std::vector<std::string> listFiles(const std::string& directory);

    // ------------------------------------------------------------
    // Join two paths safely
    // Example: joinPath("/abc", "xyz.txt") -> "/abc/xyz.txt"
    // ------------------------------------------------------------
    std::string joinPath(const std::string& base,
                         const std::string& file);

    // ------------------------------------------------------------
    // Extract filename from full path
    // ------------------------------------------------------------
    std::string getFilename(const std::string& path);

    // ------------------------------------------------------------
    // Extract file extension (lowercased, no dot)
    // ------------------------------------------------------------
    std::string getExtension(const std::string& path);
}
