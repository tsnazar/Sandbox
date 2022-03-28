#include "TestLayer.h"

#include <imgui.h>
#include <glad/glad.h>

TestLayer::TestLayer(const std::string& name)
	: Layer(name), m_Color{ 0.2, 0.3, 0.8, 1.0 } {}

TestLayer::~TestLayer()
{}

void TestLayer::OnAttach()
{}

void TestLayer::OnDetach()
{}

void TestLayer::OnUpdate(float dt)
{
	glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void TestLayer::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::ColorEdit4("Clear color", m_Color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
