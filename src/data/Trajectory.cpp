#include "Trajectory.h"
#include <iostream>

Trajectory::Trajectory()
{
    points.reserve(20000); // typical KITTI sequence size
}

void Trajectory::addPoint(const glm::vec3& p)
{
    points.push_back(p);
}

void Trajectory::clear()
{
    points.clear();
}

const std::vector<glm::vec3>& Trajectory::getPoints() const
{
    return points;
}

glm::vec3 Trajectory::getLastPoint() const
{
    if (points.empty())
        return glm::vec3(0.0f);

    return points.back();
}

bool Trajectory::isEmpty() const
{
    return points.empty();
}

size_t Trajectory::size() const
{
    return points.size();
}
