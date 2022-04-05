#pragma once
#include "Layer.h"
#include "PerspectiveCameraController.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

#include <memory>

class TestLightC : public Layer
{
public:
	TestLightC(const std::string& name = "Layer");
	~TestLightC();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

	void RenderCube();
private:
	PerspectiveCameraController m_Camera;
	std::unique_ptr<Shader> m_ObjectShader;
	std::unique_ptr<Shader> m_LightShader;

	bool m_CameraEnabled = false;

	std::unique_ptr<VertexArray> m_VAO = nullptr;

	struct Material
	{
		std::unique_ptr<Texture> diffuse;
		std::unique_ptr<Texture> specular;
		float shininess;
	};

	struct Light
	{
		float cutOff = 12.5f;
		float outerCutOff = 15.0f;

		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};

	Light m_Light;
	Material m_Material;
};