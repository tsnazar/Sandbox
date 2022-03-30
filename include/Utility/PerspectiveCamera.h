#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>

class PerspectiveCamera
{
public:
	PerspectiveCamera(float fov, float aspectRatio, float zNear = 0.1f, float zFar = 100.0f);

	void SetProjection(float fov, float aspectRatio, float zNear, float zFar);

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

	const glm::vec3& GetFront() const { return m_CameraFront; }
	const glm::vec3& GetRight() const { return m_CameraRight; }

	const std::pair<float, float>& GetAngles() const { return std::make_pair(m_CameraYaw, m_CameraPitch); }
	void SetAngles(float cameraYaw, float cameraPitch) { m_CameraYaw = cameraYaw; m_CameraPitch = cameraPitch; RecalculateCameraVectors(); RecalculateViewMatrix(); }

	const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
private:
	void RecalculateViewMatrix();
	void RecalculateCameraVectors();
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;
	glm::vec3 m_CameraRight;

	float m_CameraYaw = -90.0f, m_CameraPitch = 0.0f;
};