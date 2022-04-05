#pragma once
#include "Layer.h"
#include "Shader.h"
#include "PerspectiveCameraController.h"
#include "VertexArray.h"
#include "ShadowMapFBO.h"

#include "Texture.h"

#include <memory>

class TestPointShadows : public Layer
{
public:
	TestPointShadows(const std::string& name = "Layer");
	~TestPointShadows();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

	void RenderCube();
	void RenderQuad();
	void RenderPlane();
	void RenderScene(const Shader& shader);
private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Shader> m_ShadowDepthShader;
	PerspectiveCameraController m_Camera;

	std::unique_ptr<VertexArray> m_CubeVAO = nullptr;

	std::unique_ptr<Texture> m_Texture = nullptr;

	std::unique_ptr<ShadowMapFBO> m_ShadowMapFBO = nullptr;

	glm::mat4 m_ShadowProj;
	glm::vec3 m_LightPos = { 0.0f, 0.0f, 0.0f };

	float m_FarPlane = 25.0f;
	float m_NearPlane = 0.1f;

	bool m_CameraEnabled = false;
};