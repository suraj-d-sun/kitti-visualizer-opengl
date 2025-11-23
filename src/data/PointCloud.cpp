#include "PointCloud.h"

PointCloud::PointCloud()
{
}

PointCloud::PointCloud(const std::vector<Point3D>& pts)
    : m_points(pts)
{
}

size_t PointCloud::size() const
{
    return m_points.size();
}

bool PointCloud::empty() const
{
    return m_points.empty();
}

const std::vector<PointCloud::Point3D>& PointCloud::points() const
{
    return m_points;
}

std::vector<PointCloud::Point3D>& PointCloud::points()
{
    return m_points;
}

void PointCloud::clear()
{
    m_points.clear();
}

void PointCloud::reserve(size_t n)
{
    m_points.reserve(n);
}

void PointCloud::addPoint(const Point3D& p)
{
    m_points.push_back(p);
}

void PointCloud::addPoint(float x, float y, float z,
                          float r, float g, float b, float a)
{
    Point3D p;
    p.position = glm::vec3(x, y, z);
    p.color = glm::vec4(r, g, b, a);
    m_points.push_back(p);
}

glm::vec3 PointCloud::computeCentroid() const
{
    if (m_points.empty())
        return glm::vec3(0.0f);

    glm::vec3 sum(0.0f);
    for (const auto& p : m_points)
        sum += p.position;

    return sum / static_cast<float>(m_points.size());
}

glm::vec3 PointCloud::minBounds() const
{
    if (m_points.empty())
        return glm::vec3(0.0f);

    glm::vec3 minV = m_points[0].position;
    for (const auto& p : m_points)
        minV = glm::min(minV, p.position);

    return minV;
}

glm::vec3 PointCloud::maxBounds() const
{
    if (m_points.empty())
        return glm::vec3(0.0f);

    glm::vec3 maxV = m_points[0].position;
    for (const auto& p : m_points)
        maxV = glm::max(maxV, p.position);

    return maxV;
}
