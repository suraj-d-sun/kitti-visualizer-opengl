#include "KittiDataLoader.h"

#include "PointCloudParser.h"
#include "ImageLoader.h"
#include "PoseLoader.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"

#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
KittiDataLoader::KittiDataLoader(const std::string& basePath)
    : sequencePath(basePath)
{
    if (!fs::exists(sequencePath))
    {
        LOG_ERROR("KITTI sequence path does not exist: " + sequencePath);
        return;
    }

    LOG_INFO("Initializing KITTI loader at: " + sequencePath);

    loadFrameCount();
    loadPosesFile();
}

// ------------------------------------------------------------
// Locate total number of velodyne frames
// ------------------------------------------------------------
void KittiDataLoader::loadFrameCount()
{
    velodynePath = sequencePath + "/velodyne";

    if (!fs::exists(velodynePath))
    {
        LOG_ERROR("Velodyne folder not found: " + velodynePath);
        return;
    }

    int count = 0;
    for (auto& entry : fs::directory_iterator(velodynePath))
    {
        if (entry.path().extension() == ".bin")
            count++;
    }

    totalFrames = count;

    LOG_INFO("Found " + std::to_string(totalFrames) +
             " LiDAR frames in: " + velodynePath);
}

// ------------------------------------------------------------
// Load poses file (once at initialization)
// ------------------------------------------------------------
void KittiDataLoader::loadPosesFile()
{
    std::string posesFile = sequencePath + "/poses.txt";
    if (!fs::exists(posesFile))
    {
        LOG_WARN("No poses.txt found at: " + posesFile);
        return;
    }

    PoseLoader loader;
    poses = loader.loadAllPoses(posesFile);

    LOG_INFO("Loaded " + std::to_string(poses.size()) + " poses from poses.txt");
}

// ------------------------------------------------------------
// Load LiDAR point cloud
// ------------------------------------------------------------
PointCloud KittiDataLoader::loadPointCloud(int frameID)
{
    if (frameID < 0 || frameID >= totalFrames)
    {
        LOG_ERROR("Invalid frameID: " + std::to_string(frameID));
        return PointCloud();
    }

    std::stringstream ss;
    ss << std::setw(6) << std::setfill('0') << frameID;
    std::string file = velodynePath + "/" + ss.str() + ".bin";

    PointCloudParser parser;
    return parser.parse(file);
}

// ------------------------------------------------------------
// Load camera image
// ------------------------------------------------------------
bool KittiDataLoader::loadImage(
    int frameID,
    int& width,
    int& height,
    std::vector<unsigned char>& data)
{
    std::string imageFolder = sequencePath + "/image_2";

    if (!fs::exists(imageFolder))
    {
        LOG_ERROR("image_2 folder not found: " + imageFolder);
        return false;
    }

    std::stringstream ss;
    ss << std::setw(6) << std::setfill('0') << frameID;

    std::string imageFile = imageFolder + "/" + ss.str() + ".png";

    ImageLoader loader;
    return loader.loadImage(imageFile, width, height, data);
}

// ------------------------------------------------------------
// Load pose for frame
// ------------------------------------------------------------
glm::mat4 KittiDataLoader::loadPose(int frameID)
{
    if (frameID < 0 || frameID >= static_cast<int>(poses.size()))
    {
        LOG_WARN("Pose not available for frame: " + std::to_string(frameID));
        return glm::mat4(1.0f);
    }

    return poses[frameID];
}

// ------------------------------------------------------------
// Accessors
// ------------------------------------------------------------
int KittiDataLoader::getTotalFrames() const
{
    return totalFrames;
}

std::string KittiDataLoader::getSequencePath() const
{
    return sequencePath;
}
