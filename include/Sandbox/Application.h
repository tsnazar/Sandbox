#pragma once
#include <string>
#include <memory>

#include "Window.h"
#include "Layer.h"
#include "ImGuiLayer.h"
#include "MenuLayer.h"

class Application
{
public:
	Application(const std::string& name = "OpenGL Sandbox", unsigned int width = 1280, unsigned int height = 720);
	~Application() = default;

	void Run();

	void OnEvent(Event& event);

	template <typename T>
	void RegisterLayer(const std::string& name)
	{
		m_MenuLayer->RegisterLayer<T>(name);
	}

	inline Window& GetWindow() const { return *m_Window;  }
	inline static Application& Get() { return *s_Instance; }

private:
	bool OnWindowClose(Event& event);
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
	Layer* m_CurrentLayer = nullptr;
	MenuLayer* m_MenuLayer;

	bool m_Running = true;
	float m_LastFrameTime = 0.0f;	
private:
	static Application* s_Instance;
};
