#include "OrthographicCameraController.h"
#include "Core.h"
#include "Input.h"
#include <GLFW/glfw3.h>

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
	: m_AspectRatio(aspectRatio),m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
{
}

void OrthographicCameraController::OnUpdate(float delta)
{
	if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
		m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
	}
	if (Input::IsKeyPressed(GLFW_KEY_D))
	{
		m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
		m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
	}
	if (Input::IsKeyPressed(GLFW_KEY_W))
	{
		m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
		m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
	}
	if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
		m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * delta;
	}

	if (m_Rotation)
	{
		if (Input::IsKeyPressed(GLFW_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * delta;
		if (Input::IsKeyPressed(GLFW_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * delta;

		if (m_CameraRotation > 180.0f)
			m_CameraRotation -= 360.0f;
		else if (m_CameraRotation <= -180.0f)
			m_CameraRotation += 360.0f;

		m_Camera.SetRotation(m_CameraRotation);
	}

	m_Camera.SetPosition(m_CameraPosition);

	m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
{
	m_ZoomLevel -= event.GetYOffset() * 0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
{
	m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
