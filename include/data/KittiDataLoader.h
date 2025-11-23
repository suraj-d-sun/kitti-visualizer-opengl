#pragma once

#include "IKittiLoader.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class PointCloud;
class PointCloudParser;
class PoseLoader;
class ImageLoader;

// ------------------------------------------------------------
// KittiDataLoader
// ------------------------------------------------------------
// Implements IKittiLoader interface to load:
//   - LiDAR .bin files
//   - Camera images (PNG/JPEG)
//   - Vehicle poses
//
// This class owns:
//   • PointCloudParser
//   • PoseLoader
//   • ImageLoader
//
// Responsibilities:
//   - Build file paths for each frame
//   - Load LiDAR → PointCloud
//   - Load Image → raw pixel data
//   - Load Poses → glm::mat4
// ------------------------------------------------------------

class KittiDataLoader : public IKittiLoader
{
public:
    KittiDataLoader(const std::string& basePath, const std::string& sequence);
    ~KittiDataLoader() = default;

    // IKittiLoader overrides
    PointCloud loadPointCloud(int frameID) override;
    bool loadImage(int frameID, int& width, int& height,
                   std::vector<unsigned char>& data) override;
    glm::mat4 loadPose(int frameID) override;

    int getTotalFrames() const override { return m_totalFrames; }
    std::string getSequencePath() const override { return m_sequencePath; }

private:
    // helpers to build file paths
    std::string buildPointCloudPath(int frameID) const;
    std::string buildImagePath(int frameID) const;
    std::string buildPoseFilePath() const;

    // initialization helpers
    void scanSequence();
    void loadPoses();

private:
    std::string m_basePath;       // e.g., "data/kitti/"
    std::string m_sequenceName;   // e.g., "00"
    std::string m_sequencePath;   // basePath + "sequences/00/"

    int m_totalFrames = 0;

    // Data loaders / parsers
    std::unique_ptr<PointCloudParser> m_pointCloudParser;
    std::unique_ptr<PoseLoader>       m_poseLoader;
    std::unique_ptr<ImageLoader>      m_imageLoader;

    // Cached poses
    std::vector<glm::mat4> m_poses;
};
