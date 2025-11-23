#include "utils/MathUtils.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace MathUtils
{
	glm::quat eulerToQuat(const glm::vec3& eulerDeg)
	{
		glm::vec3 rad = glm::radians(eulerDeg);
		return glm::quat(rad);
	}

	glm::mat4 quatToMatrix(const glm::quat& q)
	{
		return glm::toMat4(q);
	}

	glm::vec3 safeNormalize(const glm::vec3& v)
	{
		float len = glm::length(v);
		return (len > 1e-6f) ? v / len : glm::vec3(0.0f);
	}

	glm::mat4 computeLookAt(
		const glm::vec3& position,
		const glm::vec3& target,
		const glm::vec3& up)
	{
		return glm::lookAt(position, target, up);
	}

	float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	glm::vec3 extractTranslation(const glm::mat4& m)
	{
		return glm::vec3(m[3]);
	}

	float extractYaw(const glm::mat4& pose)
	{
		// yaw = atan2(R21, R11)
		return std::atan2(pose[0][2], pose[0][0]);
	}
}
