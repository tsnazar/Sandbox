#pragma once
#include "Layer.h"
#include "MouseEvent.h"

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
	bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
private:
	float m_Time = 0.0f;
};