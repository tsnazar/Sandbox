#include "TestLightD.h"
#include "Core.h"
#include "Application.h"

#include <imgui.h>

#include <GLFW/glfw3.h>


TestLightD::TestLightD(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
	m_Camera.GetCamera().SetPosition({ 0, 0, 3 });
}

TestLightD::~TestLightD()
{}

void TestLightD::OnAttach()
{
	m_ObjectShader = std::make_unique<Shader>("layers/TestLightD/res/bLightVertD.glsl", "layers/TestLightD/res/bLightFragD.glsl");
	m_LightShader = std::make_unique<Shader>("layers/TestLightD/res/basicVertex.glsl", "layers/TestLightD/res/basicFragment.glsl");

	m_Material.Diffuse = std::make_unique<Texture>("res/textures/container2.png");
	m_Material.Specular = std::make_unique<Texture>("res/textures/container2_specular.png");
	m_Material.Shininess = 64;

	GLCall(glEnable(GL_DEPTH_TEST));
}

void TestLightD::OnDetach()
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void TestLightD::OnUpdate(float dt)
{
	if(m_CameraEnabled)
		m_Camera.OnUpdate(dt);

	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	{	
		m_ObjectShader->Bind();
		m_ObjectShader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
		m_ObjectShader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());

		m_ObjectShader->SetUniform3f("u_DirLight.Direction", m_DirLight.Direction);
		m_ObjectShader->SetUniform3f("u_DirLight.Ambient", m_DirLight.Ambient);
		m_ObjectShader->SetUniform3f("u_DirLight.Diffuse", m_DirLight.Diffuse);
		m_ObjectShader->SetUniform3f("u_DirLight.Specular", m_DirLight.Specular);

		for (unsigned int i = 0; i < 4; i++)
		{
			m_ObjectShader->SetUniform3f("u_PointLight[" + std::to_string(i) + "].Position", pointLightPositions[i]);
			m_ObjectShader->SetUniform3f("u_PointLight[" + std::to_string(i) + "].Ambient", m_PositionLight.Ambient);
			m_ObjectShader->SetUniform3f("u_PointLight[" + std::to_string(i) + "].Diffuse", m_PositionLight.Diffuse);
			m_ObjectShader->SetUniform3f("u_PointLight[" + std::to_string(i) + "].Specular", m_PositionLight.Specular);
			m_ObjectShader->SetUniform1f("u_PointLight[" + std::to_string(i) + "].Constant", m_PositionLight.Constant);
			m_ObjectShader->SetUniform1f("u_PointLight[" + std::to_string(i) + "].Linear", m_PositionLight.Linear);
			m_ObjectShader->SetUniform1f("u_PointLight[" + std::to_string(i) + "].Quadratic", m_PositionLight.Quadratic);
		}

		m_ObjectShader->SetUniform1i("u_Material.Diffuse", 0);
		m_Material.Diffuse->Bind(0);
		m_ObjectShader->SetUniform1i("u_Material.Specular", 1);
		m_Material.Specular->Bind(1);
		m_ObjectShader->SetUniform1f("u_Material.Shininess", m_Material.Shininess);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(0.5));
			m_ObjectShader->SetUniformMat4f("u_Model", model);

			RenderCube();
		}
	}

	{
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 MVP = m_Camera.GetCamera().GetViewProjectionMatrix() * glm::translate(glm::mat4(1.0f), pointLightPositions[i]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
			m_LightShader->Bind();
			m_LightShader->SetUniformMat4f("u_MVP", MVP);
			m_LightShader->SetUniform4f("u_Color", glm::vec4(1.0f));

			RenderCube();
		}
	}
}

void TestLightD::OnEvent(Event& event)
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

	if(m_CameraEnabled)
		m_Camera.OnEvent(event);
}

void TestLightD::OnImGuiRender()
{
	ImGui::Text("Press F to enable/disable camera view");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestLightD::RenderCube()
{
	if (m_VAO == nullptr)
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

		m_VAO = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout = {
			{VertexBufferElementType::Float3, "a_Position"},
			{VertexBufferElementType::Float3, "a_Normal"},
			{VertexBufferElementType::Float2, "a_TexCoords"},
		};
		VBO->SetLayout(layout);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_VAO->AddVertexBuffer(VBO);
		m_VAO->SetIndexBuffer(indexBuffer);

	}
	// render Cube
	m_VAO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
	m_VAO->UnBind();
}