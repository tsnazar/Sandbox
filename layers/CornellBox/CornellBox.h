#pragma once
#include "Layer.h"
#include "PerspectiveCameraController.h"
#include "Shader.h"
#include "Texture.h"
#include "ShadowMapFBO.h"

#include "VertexArray.h"

#include <memory>

class CornellBox : public Layer
{
public:
	CornellBox(const std::string& name = "Layer");
	~CornellBox();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

	void RenderScene();
	void RenderQuad();
private:
	PerspectiveCameraController m_Camera;
	std::unique_ptr<Shader> m_ObjectShader = nullptr;
	std::unique_ptr<Shader> m_LightShader = nullptr;
	std::unique_ptr<Shader> m_ShadowDepthShader = nullptr;

	std::unique_ptr<Shader> m_ScreenShader = nullptr;

	std::unique_ptr<VertexArray> m_VertexArray = nullptr;
	std::unique_ptr<VertexArray> m_QuadVAO = nullptr;

	std::unique_ptr<ShadowMapFBO> m_ShadowMapFBO;

	struct DirectionLight
	{
		glm::vec3 Position = { 343.0f, 548.0f, 227.0f };
		glm::vec3 Ambient = { 0.05f, 0.05f, 0.05f };
		glm::vec3 Diffuse = { 0.8f, 0.8f, 0.8f };
		glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };

		float Constant = 1.0f;
		float Linear = 0.0014f;
		float Quadratic = 0.00007f;
	};

	DirectionLight m_Light;

	bool m_CameraEnabled = false;
	bool m_Blinn = false;
};