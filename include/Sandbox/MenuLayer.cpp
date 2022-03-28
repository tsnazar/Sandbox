#include "MenuLayer.h"

#include <imgui.h>
#include <glad/glad.h>

MenuLayer::MenuLayer(Layer*& currentLayerPointer)
	: m_CurrentLayer(currentLayerPointer)
{}

void MenuLayer::OnUpdate(float delta)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void MenuLayer::OnImGuiRender()
{
	ImGui::Begin("Test");
	for (auto& layer : m_Layers)
	{
		if (ImGui::Button(layer.first.c_str()))
		{
			m_CurrentLayer = layer.second();
			m_CurrentLayer->OnAttach();
		}
	}
	ImGui::End();
}
