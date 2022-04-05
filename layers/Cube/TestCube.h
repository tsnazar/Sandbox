#pragma once
#include "Layer.h"
#include "Shader.h"
#include "PerspectiveCameraController.h"

#include <memory>

class TestCube : public Layer
{
public:
	TestCube(const std::string& name = "Layer");
	~TestCube();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;
private:
	std::unique_ptr<Shader> m_Shader;
	PerspectiveCameraController m_Camera;

	bool m_CameraEnabled = false;

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
};