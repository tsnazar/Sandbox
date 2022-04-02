#pragma once
#include "PerspectiveCamera.h"
#include "Event.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"

class PerspectiveCameraController
{
public:
	PerspectiveCameraController(float fov, float aspectRatio, float zNear = 0.1f, float zFar = 100.0f);
	~PerspectiveCameraController();

	void OnUpdate(float delta);
	void OnEvent(Event& event);

	void SetFirstMouse(bool state) { m_FirstMouse = state; }

	PerspectiveCamera& GetCamera() { return m_Camera; }
	const PerspectiveCamera& GetCamera() const { return m_Camera; }
private:
	bool OnMouseMoved(MouseMovedEvent& event);
	bool OnWindowResized(WindowResizeEvent& event);
private:
	float m_Fov;
	float m_AspectRatio;

	PerspectiveCamera m_Camera;

	float m_CameraYaw, m_CameraPitch;
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraVelocity = 5.0f;

	bool m_FirstMouse = true;
};