#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "Application.h"
#include "Core.h"
#include "TestLayer.h"
#include "ApplicationEvent.h"

#include <iostream>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name /*= "OpenGL Sandbox"*/, unsigned int width /*= 1280*/, unsigned int height /*= 720*/)
	: m_MenuLayer(new MenuLayer(m_CurrentLayer))
{
	if (!s_Instance)
	{
		// init logging
	}

	GLCORE_ASSERT(!s_Instance);
	s_Instance = this;

	m_Window = std::make_unique<Window>(WindowProps{ name, width, height });
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	m_ImGuiLayer = std::make_unique<ImGuiLayer>();
	m_ImGuiLayer->OnAttach();

	m_CurrentLayer = m_MenuLayer;
}

void Application::Run()
{
	while (m_Running)
	{
		float time = (float)glfwGetTime();
		float delta = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if(m_CurrentLayer)
			m_CurrentLayer->OnUpdate(delta);

		m_ImGuiLayer->Begin();
		if (m_CurrentLayer)
		{
			if (m_CurrentLayer != m_MenuLayer && ImGui::Button("<-"))
			{
				m_CurrentLayer->OnDetach();
				delete m_CurrentLayer;
				m_CurrentLayer = m_MenuLayer;
			}
			m_CurrentLayer->OnImGuiRender();
		}
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	if (event.Handled)
		return;
	
	m_ImGuiLayer->OnEvent(event);
	if (event.Handled)
		return;

	if(m_CurrentLayer != nullptr)
		m_CurrentLayer->OnEvent(event);
}

bool Application::OnWindowClose(Event& event)
{
	if (m_CurrentLayer != m_MenuLayer)
		delete m_MenuLayer;

	delete m_CurrentLayer;
	m_CurrentLayer = nullptr;
	
	m_ImGuiLayer->OnDetach();
	m_Running = false;
	return true;
}
