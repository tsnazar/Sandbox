#pragma once
#include <string>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Event.h"

struct WindowProps
{
	std::string Title;
	unsigned int Width, Height;

	WindowProps(const std::string& title = "OpenGL Sandbox",
		unsigned int width = 1280, unsigned int height = 780)
		: Title(title), Width(width), Height(height) {}
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>; // add Event

	Window(const WindowProps& props);
	~Window();

	void OnUpdate();

	inline unsigned int GetWidth() const { return m_Data.Width; }
	inline unsigned int GetHeight() const { return m_Data.Height; }

	inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	
	void SetVsync(bool enabled = false);
	bool IsVsync() const;

	void SetCursorHidden(bool enabled = false);
	bool IsCursorHidden() const;

	void* GetNativeWindow() const { return m_Window; }

	static Window* Create(const WindowProps& props = WindowProps());
private:
	void Init(const WindowProps& props);
	void Shutdown();
private:
	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync, CursorHidden;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};