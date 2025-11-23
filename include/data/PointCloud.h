#pragma once

#include <vector>
#include <cstdint>

// ------------------------------------------------------------
// PointCloud
// ------------------------------------------------------------
// Represents a single KITTI LiDAR frame.
// Each point has:
//   - x, y, z  (float)
//   - intensity (float)
//
// NOTE:
//  • Simple data container — no parsing logic here.
//  • Parsing is done by PointCloudParser.
//  • Renderer receives PointCloud directly.
// ------------------------------------------------------------

class PointCloud
{
public:
    struct Point
    {
        float x;
        float y;
        float z;
        float intensity;
    };

public:
    PointCloud() = default;
    ~PointCloud() = default;

    // Set full point list at once
    void setPoints(const std::vector<Point>& pts)
    {
        m_points = pts;
    }

    // Move assignment for efficiency
    void setPoints(std::vector<Point>&& pts)
    {
        m_points = std::move(pts);
    }

    // Read-only access to point list
    const std::vector<Point>& getPoints() const { return m_points; }

    // Direct push (optional convenience)
    void addPoint(float x, float y, float z, float intensity)
    {
        m_points.push_back({x, y, z, intensity});
    }

    size_t size() const { return m_points.size(); }

private:
    std::vector<Point> m_points;
};
