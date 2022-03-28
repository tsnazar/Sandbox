#pragma once
#include "Layer.h"

class TestLayer : public Layer
{
public:
	TestLayer(const std::string& name = "Layer");
	~TestLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnImGuiRender() override;
private:
	float m_Color[4];
};