#pragma once
#include "Layer.h"
#include "Shader.h"
#include "PerspectiveCameraController.h"
#include "VertexArray.h"
#include "ShadowMapFBO.h"

#include "Texture.h"

#include <memory>

class TestShadowMap : public Layer
{
public:
	TestShadowMap(const std::string& name = "Layer");
	~TestShadowMap();

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
	std::unique_ptr<Shader> m_ScreenShader;
	std::unique_ptr<Shader> m_ShadowDepthShader;
	PerspectiveCameraController m_Camera;

	std::unique_ptr<VertexArray> m_CubeVAO = nullptr;
	std::unique_ptr<VertexArray> m_QuadVAO = nullptr;
	std::unique_ptr<VertexArray> m_PlaneVAO = nullptr;

	std::unique_ptr<Texture> m_Texture;

	std::unique_ptr<ShadowMapFBO> m_ShadowMapFBO = nullptr;

	unsigned int cubeVAO;

	bool m_CameraEnabled = false;

};