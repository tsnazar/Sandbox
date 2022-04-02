#include "PerspectiveCameraController.h"
#include "Input.h"
#include "Core.h"
#include "Application.h"

#include <GLFW/glfw3.h>

PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear /*= 0.1f*/, float zFar /*= 100.0f*/)
	: m_Fov(fov), m_AspectRatio(aspectRatio), m_Camera(fov, aspectRatio, zNear, zFar), m_CameraPosition(m_Camera.GetPosition())
{
	auto angles = m_Camera.GetAngles();
	m_CameraYaw = angles.first;
	m_CameraPitch = angles.second;
}

PerspectiveCameraController::~PerspectiveCameraController()
{
	Application::Get().GetWindow().SetCursorHidden(false);
}

void PerspectiveCameraController::OnUpdate(float delta)
{
	m_CameraPosition = m_Camera.GetPosition();

	if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		m_CameraPosition -= delta * m_CameraVelocity * m_Camera.GetRight();
	}
	if (Input::IsKeyPressed(GLFW_KEY_D))
	{
		m_CameraPosition += delta * m_CameraVelocity * m_Camera.GetRight();
	}
	if (Input::IsKeyPressed(GLFW_KEY_W))
	{
		m_CameraPosition += delta * m_CameraVelocity * m_Camera.GetFront();
	}
	if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		m_CameraPosition -= delta * m_CameraVelocity * m_Camera.GetFront();
	}

	m_Camera.SetPosition(m_CameraPosition);
}

void PerspectiveCameraController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
	dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
}

bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& event)
{
	static float lastX = 0, lastY = 0;

	if (m_FirstMouse)
	{
		lastX = event.GetX();
		lastY = event.GetY();
		m_FirstMouse = false;
	}
	float offsetX = event.GetX() - lastX;
	float offsetY = event.GetY() - lastY;

	lastX = event.GetX();
	lastY = event.GetY();

	auto angles = m_Camera.GetAngles();
	m_CameraYaw = angles.first;
	m_CameraPitch = angles.second;

	m_CameraYaw += 0.1 * offsetX;
	m_CameraPitch += 0.1 * offsetY;
	if (m_CameraPitch > 89.0f)
		m_CameraPitch = 89.0f;
	if (m_CameraPitch < -89.0f)
		m_CameraPitch = -89.0f;
	
	m_Camera.SetAngles(m_CameraYaw, m_CameraPitch);
	return false;
}

bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& event)
{
	m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
	if (std::isnan(m_AspectRatio))
		m_AspectRatio = 1;
	m_Camera.SetProjection(m_Fov, m_AspectRatio, 0.1, 100.0f);
	return false;
}
