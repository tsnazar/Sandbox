#include "Window.h"
#include "Core.h"
#include "Application.h"
#include "Event.h"

#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowProps& props /*= WindowProps()*/)
{
	return new Window(props);
}

Window::Window(const WindowProps& props)
{
	Init(props);
}

Window::~Window()
{
	Shutdown();
}

void Window::Init(const WindowProps& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	if (!s_GLFWInitialized)
	{
		int success = glfwInit();
		SANDBOX_ASSERT(success);
		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	SANDBOX_ASSERT(status);

	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVsync(true);

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			glViewport(0, 0, width, height);

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
	{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
	});
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}

void Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::SetVsync(bool enabled /*= false*/)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
	m_Data.VSync = enabled;
}

bool Window::IsVsync() const
{
	return m_Data.VSync;
}

void Window::SetCursorHidden(bool enabled /*= false*/)
{
	if (enabled)
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else 
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_Data.CursorHidden = enabled;
}

bool Window::IsCursorHidden() const
{
	return m_Data.CursorHidden;
}
