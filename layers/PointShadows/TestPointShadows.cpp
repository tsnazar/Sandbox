#include "TestPointShadows.h"
#include "Core.h"
#include "KeyEvent.h"
#include "Application.h"

#include <imgui.h>
#include "GLFW/glfw3.h"

float SHADOW_WIDTH = 1024;
float SHADOW_HEIGHT = 1024;

struct CameraDirection
{
	GLenum CubemapFace;
	glm::vec3 Target;
	glm::vec3 Up;
};

CameraDirection gCameraDirections[6] =
{
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

TestPointShadows::TestPointShadows(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
	m_ShadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, m_NearPlane, m_FarPlane);
}

TestPointShadows::~TestPointShadows()
{}

void TestPointShadows::OnAttach()
{
	m_Shader = std::make_unique<Shader>("layers/PointShadows/res/shadowVert.glsl", "layers/PointShadows/res/shadowFrag.glsl");
	m_ShadowDepthShader = std::make_unique<Shader>("layers/PointShadows/res/shadowDepthVert.glsl", "layers/PointShadows/res/shadowDepthFrag.glsl", "layers/PointShadows/res/shadowDepthGeom.glsl");

	m_Texture = std::make_unique<Texture>("res/textures/container2.png");

	m_ShadowMapFBO = std::make_unique<ShadowMapFBO>(ShadowMapType::CUBEMAP, SHADOW_WIDTH, SHADOW_HEIGHT);

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
}

void TestPointShadows::OnDetach()
{
	GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDisable(GL_DEPTH_TEST));
}

void TestPointShadows::OnUpdate(float dt)
{

	if (m_CameraEnabled)
		m_Camera.OnUpdate(dt);

	m_ShadowDepthShader->Bind();
	m_ShadowDepthShader->SetUniform1f("u_FarPlane", m_FarPlane);
	m_ShadowDepthShader->SetUniform3f("u_LightPos", m_LightPos);	
	for (unsigned int i = 0; i < 6; i++)
	{
		m_ShadowDepthShader->SetUniformMat4f("u_ShadowMatrices[" + std::to_string(i) + "]", m_ShadowProj * glm::lookAt(m_LightPos, gCameraDirections[i].Target, gCameraDirections[i].Up));
	}

	m_ShadowMapFBO->Bind(); // bind depth fbo
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	RenderScene(*m_ShadowDepthShader); // render scene in fbo
	m_ShadowMapFBO->Unbind();

	// shadowMapFbo changes viewport, so we need to change it back
	GLCall(glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	m_Shader->Bind(); // bind shader for scene
	m_Shader->SetUniform1i("u_DiffuseTexture", 0);
	m_Texture->Bind(); // bind cube texture 
	m_Shader->SetUniform1i("u_DepthMap", 1);
	m_ShadowMapFBO->BindDepthTexture(1); // bind depth texture
	m_Shader->SetUniform1f("u_FarPlane", m_FarPlane);
	m_Shader->SetUniform3f("u_LightPos", m_LightPos);
	m_Shader->SetUniform3f("u_ViewPos", m_Camera.GetCamera().GetPosition());
	m_Shader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
	m_Shader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());
	RenderScene(*m_Shader);
}

void TestPointShadows::OnEvent(Event& event)
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

void TestPointShadows::OnImGuiRender()
{
	ImGui::Text("Press F to enable\disable camera view");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestPointShadows::RenderScene(const Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f));
	shader.SetUniformMat4f("u_Model", model);
	glDisable(GL_CULL_FACE); // render outer room cube
	shader.SetUniform1i("u_ReverseNormals", 1); // reverse normals for light to work
	RenderCube();
	shader.SetUniform1i("u_ReverseNormals", 0); 
	glEnable(GL_CULL_FACE);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetUniformMat4f("u_Model", model);
	RenderCube();
}

void TestPointShadows::RenderCube()
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
