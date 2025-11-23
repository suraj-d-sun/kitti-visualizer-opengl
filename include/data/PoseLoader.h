#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

// ------------------------------------------------------------
// PoseLoader
// ------------------------------------------------------------
// Responsible for loading vehicle poses from KITTI pose files.
//
// KITTI pose format (poses/<sequence>.txt):
//   Each line contains 12 floats representing a 3×4 matrix:
//        r11 r12 r13 tx
//        r21 r22 r23 ty
//        r31 r32 r33 tz
//
//   We convert this into a full 4×4 glm::mat4.
//
// Responsibilities:
//   ✓ Load all poses from the file
//   ✓ Provide pose for a given frame
//   ✓ Convert KITTI 3×4 to 4×4 homogeneous matrix
//
// Completely follows SRP (Single Responsibility Principle).
// ------------------------------------------------------------

class PoseLoader
{
public:
    PoseLoader() = default;
    ~PoseLoader() = default;

    // Loads all poses from a KITTI pose file.
    // Example file: "poses/00.txt"
    //
    // Returns:
    //   true  -> loaded successfully
    //   false -> file not found / parse error
    bool loadPoseFile(const std::string& filepath);

    // Returns the pose of a given frame.
    // If out of range → identity matrix.
    glm::mat4 getPose(int frameID) const;

    // Number of poses
    int getPoseCount() const { return static_cast<int>(m_poses.size()); }

private:
    glm::mat4 convertToMat4(const std::vector<float>& values) const;

private:
    std::vector<glm::mat4> m_poses;
};
