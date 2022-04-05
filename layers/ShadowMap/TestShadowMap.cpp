#include "TestShadowMap.h"
#include "Core.h"
#include "KeyEvent.h"
#include "Application.h"

#include <imgui.h>
#include "GLFW/glfw3.h"

TestShadowMap::TestShadowMap(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
}

TestShadowMap::~TestShadowMap()
{}

void TestShadowMap::OnAttach()
{
	m_Shader = std::make_unique<Shader>("layers/ShadowMap/res/shadowVert.glsl", "layers/ShadowMap/res/shadowFrag.glsl");
	//m_ScreenShader = std::make_unique<Shader>("layers/ShadowMap/res/shadowDebugVert.glsl", "layers/ShadowMap/res/shadowDebugFrag.glsl");
	m_ShadowDepthShader = std::make_unique<Shader>("layers/ShadowMap/res/shadowDepthVert.glsl", "layers/ShadowMap/res/shadowDepthFrag.glsl");

	m_Texture = std::make_unique<Texture>("res/textures/container2.png");

	m_ShadowMapFBO = std::make_unique<ShadowMapFBO>(ShadowMapType::TEXTURE2D,1024, 1024);

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
}

void TestShadowMap::OnDetach()
{
	GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDisable(GL_DEPTH_TEST));
}

void TestShadowMap::OnUpdate(float dt)
{
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

	if (m_CameraEnabled)
		m_Camera.OnUpdate(dt);

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	m_ShadowMapFBO->Bind();
	m_ShadowDepthShader->Bind();
	m_ShadowDepthShader->SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMatrix);
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	RenderScene(*m_ShadowDepthShader);
	m_ShadowMapFBO->Unbind();

	GLCall(glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	/*m_ScreenShader->Bind();
	m_ScreenShader->SetUniform1i("u_DepthMap", 0);
	m_ShadowMapFBO->BindTexture(0);
	RenderQuad();*/
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_DiffuseTexture", 0);
	m_Texture->Bind();
	m_Shader->SetUniform1i("u_DepthMap", 1);
	m_ShadowMapFBO->BindDepthTexture(1);
	m_Shader->SetUniform3f("u_LightPos", lightPos);
	m_Shader->SetUniform3f("u_ViewPos", m_Camera.GetCamera().GetPosition());
	m_Shader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
	m_Shader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());
	m_Shader->SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMatrix);
	RenderScene(*m_Shader);

}

void TestShadowMap::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
		if (event.GetKeyCode() == GLFW_KEY_F)
		{
			m_CameraEnabled = !m_CameraEnabled;
			Application::Get().GetWindow().SetCursorHidden(m_CameraEnabled);
			m_Camera.SetFirstMouse(true);
		}
		return true;
	});

	if (m_CameraEnabled)
		m_Camera.OnEvent(event);
}

void TestShadowMap::OnImGuiRender()
{
	ImGui::Text("Press F to enable\disable camera view");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestShadowMap::RenderScene(const Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetUniformMat4f("u_Model", model);
	RenderPlane();
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
}

void TestShadowMap::RenderCube()
{
	if (m_CubeVAO == nullptr)
	{
		float vertices[] = {
			// back face
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right 0
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right 1         
		   -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left 2
		   -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left 3
		   // front face
		   -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left 0
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right 1
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right 2
		   -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left 3
		   // left face
		   -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right 0
		   -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left 1
		   -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left 2
		   -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right 3
		   // right face
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right 0
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right     1    
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left 2
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left 3      
		   // bottom face
		   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right 0
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left 1
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left 2
		   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right 3
		   // top face
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right 0
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right   1
		   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left 2
		   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // bottom-left 3    		
		};

		unsigned int indices[6 * 6];
		for (unsigned int i = 0; i < 6; i++)
		{
			indices[6 * i] = 0 + i * 4;
			indices[6 * i + 1] = 1 + i * 4;
			indices[6 * i + 2] = 2 + i * 4;
			indices[6 * i + 3] = 2 + i * 4;
			indices[6 * i + 4] = 3 + i * 4;
			indices[6 * i + 5] = 0 + i * 4;
		}

		m_CubeVAO = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout = {
			{VertexBufferElementType::Float3, "a_Position"},
			{VertexBufferElementType::Float3, "a_Normal"},
			{VertexBufferElementType::Float2, "a_TexCoords"},
		};
		VBO->SetLayout(layout);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_CubeVAO->AddVertexBuffer(VBO);
		m_CubeVAO->SetIndexBuffer(indexBuffer);

	}
	// render Cube
	m_CubeVAO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_CubeVAO->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
	m_CubeVAO->UnBind();
}


void TestShadowMap::RenderQuad()
{
	if (m_QuadVAO == nullptr)
	{
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		unsigned int quadIndices[] = {
			0, 1, 2,
			0, 2, 3
		};

		m_QuadVAO = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> quadVBO = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));

		VertexBufferLayout quadLayout = { {VertexBufferElementType::Float2, "a_Position"}, {VertexBufferElementType::Float2, "a_TexCoords"} };
		quadVBO->SetLayout(quadLayout);

		std::shared_ptr<IndexBuffer> quadIBO = std::make_shared<IndexBuffer>(quadIndices, sizeof(quadIndices) / sizeof(unsigned int));

		m_QuadVAO->AddVertexBuffer(quadVBO);
		m_QuadVAO->SetIndexBuffer(quadIBO);
	}
	m_QuadVAO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_QuadVAO->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
	m_QuadVAO->UnBind();
}

void TestShadowMap::RenderPlane()
{
	if (m_PlaneVAO == nullptr)
	{
		float planeVertices[] = {
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};

		unsigned int quadIndices[] = {
			0, 1, 2,
			0, 2, 3
		};

		m_PlaneVAO = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> planeVBO = std::make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));

		VertexBufferLayout planeLayout = { 
			{VertexBufferElementType::Float3, "a_Position"},
			{VertexBufferElementType::Float3, "a_Normal"}, 
			{VertexBufferElementType::Float2, "a_TexCoords"}
		};
		planeVBO->SetLayout(planeLayout);

		std::shared_ptr<IndexBuffer> planeIBO = std::make_shared<IndexBuffer>(quadIndices, sizeof(quadIndices) / sizeof(unsigned int));

		m_PlaneVAO->AddVertexBuffer(planeVBO);
		m_PlaneVAO->SetIndexBuffer(planeIBO);
	}
	m_PlaneVAO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_PlaneVAO->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
	m_PlaneVAO->UnBind();
}

