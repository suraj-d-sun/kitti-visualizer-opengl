#pragma once

#include <string>
#include <vector>
#include <memory>

class PointCloud;

// ------------------------------------------------------------
// PointCloudParser
// ------------------------------------------------------------
// Responsible ONLY for parsing KITTI's LiDAR .bin files.
//
// KITTI Velodyne binary format:
//   Each point = 4 × float32  (x, y, z, intensity)
//
// Responsibilities:
//   ✓ Read binary file
//   ✓ Convert raw float buffer → PointCloud object
//   ✓ No OpenGL, no rendering here
//
// This follows SRP (Single Responsibility Principle).
// ------------------------------------------------------------

class PointCloudParser
{
public:
    PointCloudParser() = default;
    ~PointCloudParser() = default;

    // Parses a KITTI .bin LiDAR file and returns a PointCloud object.
    // Throws std::runtime_error on file read errors.
    PointCloud parseBinFile(const std::string& filepath);
};
