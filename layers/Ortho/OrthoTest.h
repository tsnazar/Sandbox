#pragma once
#include "Layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "Shader.h"
#include "OrthographicCameraController.h"

class OrthoTest : public Layer
{
public:
	OrthoTest();
	~OrthoTest();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float deltaTime) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;
private:
	std::unique_ptr<Shader> m_Shader;
	OrthographicCameraController m_Camera;

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;
	glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;
};
