#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class PointCloud;

// ------------------------------------------------------------
// IKittiLoader Interface
// ------------------------------------------------------------
// Responsible for loading:
//   - LiDAR point clouds (.bin)
//   - Images (.png / .jpg)
//   - Vehicle poses (.txt)
//   - Sequence metadata
//
// This interface follows the Dependency Inversion Principle (DIP).
// Applications depend only on this interface, not on file formats.
// ------------------------------------------------------------

class IKittiLoader
{
public:
    virtual ~IKittiLoader() = default;

    // Load LiDAR point cloud for the given frame index
    virtual PointCloud loadPointCloud(int frameID) = 0;

    // Load camera image for given frame index (RGB raw pixel array)
    virtual bool loadImage(int frameID, int& width, int& height, std::vector<unsigned char>& data) = 0;

    // Load vehicle pose for given frame index (4x4 transformation)
    virtual glm::mat4 loadPose(int frameID) = 0;

    // Total number of frames in the KITTI sequence
    virtual int getTotalFrames() const = 0;

    // Base directory for KITTI sequence
    virtual std::string getSequencePath() const = 0;
};
