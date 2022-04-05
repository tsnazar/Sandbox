#pragma once
#include "Layer.h"
#include "PerspectiveCameraController.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

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

	void RenderCube();
private:
	PerspectiveCameraController m_Camera;
	std::unique_ptr<Shader> m_ObjectShader;
	std::unique_ptr<Shader> m_LightShader;

	bool m_CameraEnabled = false;

	std::unique_ptr<VertexArray> m_VAO;

	struct Material
	{
		std::unique_ptr<Texture> Diffuse;
		std::unique_ptr<Texture> Specular;
		float Shininess;
	};

	struct DirLight
	{
		glm::vec3 Direction = { -0.2f, -1.0f, -0.3f };

		glm::vec3 Ambient = { 0.05f, 0.05f, 0.05f };
		glm::vec3 Diffuse = { 0.4f, 0.4f, 0.4f };
		glm::vec3 Specular = { 0.5f, 0.5f, 0.5f };
	};

	struct PositionLight
	{
		glm::vec3 Ambient = { 0.05f, 0.05f, 0.05f };
		glm::vec3 Diffuse = { 0.8f, 0.8f, 0.8f };
		glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };

		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	};

	PositionLight m_PositionLight;
	DirLight m_DirLight;
	Material m_Material;
};