#pragma once

#include <vector>
#include <glm/glm.hpp>

// ------------------------------------------------------------
// Trajectory
// ------------------------------------------------------------
// Maintains the vehicle's driven path over time.
//
// Responsibilities:
//   ✓ Store sequence of 3D positions
//   ✓ Add new points each frame
//   ✓ Provide path to renderer
//   ✓ Clear / reset path
//
// This class contains no rendering logic (SRP).
// ------------------------------------------------------------

class Trajectory
{
public:
    Trajectory() = default;
    ~Trajectory() = default;

    // Add a new 3D point to the trajectory
    void addPoint(const glm::vec3& p)
    {
        m_points.push_back(p);
    }

    // Clear entire path
    void clear()
    {
        m_points.clear();
    }

    // Retrieve full path
    const std::vector<glm::vec3>& getPath() const
    {
        return m_points;
    }

    // Number of stored trajectory points
    size_t size() const { return m_points.size(); }

private:
    std::vector<glm::vec3> m_points;
};
