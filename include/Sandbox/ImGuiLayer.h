#pragma once
#include "Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void Begin();
	void End();

	void OnEvent(Event& event) override;
	bool OnMouseButtonPressed(); // add event
private:
	float m_Time = 0.0f;
};