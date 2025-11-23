#include "PoseLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

PoseLoader::PoseLoader(const std::string& poseFile)
{
    loadAllPoses(poseFile);
}

bool PoseLoader::loadAllPoses(const std::string& poseFile)
{
    std::ifstream file(poseFile);
    if (!file.is_open())
    {
        std::cerr << "[PoseLoader] Failed to open pose file: " << poseFile << std::endl;
        return false;
    }

    std::string line;
    poses.clear();

    // KITTI format:
    // each line contains 12 floats (3x4 matrix)
    // we convert it to a full 4x4 matrix
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        glm::mat4 M(1.0f);

        // Read 12 elements row-major for 3x4 matrix
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                float v;
                ss >> v;
                M[row][col] = v;
            }
        }

        // Complete last row: [0 0 0 1]
        M[3][0] = 0.0f;
        M[3][1] = 0.0f;
        M[3][2] = 0.0f;
        M[3][3] = 1.0f;

        poses.push_back(M);
    }

    file.close();
    return true;
}

glm::mat4 PoseLoader::getPose(int frameID) const
{
    if (frameID < 0 || frameID >= static_cast<int>(poses.size()))
    {
        std::cerr << "[PoseLoader] Requested invalid pose index: " << frameID << std::endl;
        return glm::mat4(1.0f);
    }
    return poses[frameID];
}

int PoseLoader::getTotalPoses() const
{
    return static_cast<int>(poses.size());
}
