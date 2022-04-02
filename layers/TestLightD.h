#pragma once
#include "Layer.h"
#include "PerspectiveCameraController.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

class TestLightD : public Layer
{
public:
	TestLightD(const std::string& name = "Layer");
	~TestLightD();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

private:
	PerspectiveCameraController m_Camera;
	std::unique_ptr<Shader> m_ObjectShader;
	std::unique_ptr<Shader> m_LightShader;

	bool m_CameraEnabled = false;

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;	

	struct Material
	{
		std::unique_ptr<Texture> diffuse;
		std::unique_ptr<Texture> specular;
		float shininess;
	};

	struct DirLight
	{
		glm::vec3 direction = { -0.2f, -1.0f, -0.3f };

		glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
		glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
		glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
	};

	struct PositionLight
	{
		glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
		glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};

	PositionLight m_PositionLight;
	DirLight m_DirLight;
	Material m_Material;
};