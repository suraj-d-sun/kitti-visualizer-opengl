#include "PointCloudParser.h"
#include "PointCloud.h"
#include <fstream>
#include <iostream>

PointCloud PointCloudParser::loadKittiBin(const std::string& filePath, bool applyDefaultColor)
{
    PointCloud cloud;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "[PointCloudParser] Failed to open .bin file: " << filePath << std::endl;
        return cloud; // empty
    }

    // KITTI velodyne binary format:
    // Each point = float x, y, z, intensity   → 4 floats = 16 bytes
    // File contains N such points.
    struct KittiPointRaw
    {
        float x, y, z, intensity;
    };

    KittiPointRaw point;

    // Pre-allocate approximate size (optional performance improvement)
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    size_t pointCount = fileSize / sizeof(KittiPointRaw);
    file.seekg(0, std::ios::beg);

    cloud.reserve(pointCount);

    while (file.read(reinterpret_cast<char*>(&point), sizeof(KittiPointRaw)))
    {
        // KITTI intensity range is [0, 1] in practice but may exceed slightly
        float I = point.intensity;

        // Color encoding — grayscale mapping from intensity
        float r = I;
        float g = I;
        float b = I;
        float a = 1.0f;

        if (!applyDefaultColor)
        {
            // Keep as grayscale; advanced color schemes may be added later
        }

        cloud.addPoint(point.x, point.y, point.z, r, g, b, a);
    }

    file.close();
    return cloud;
}
