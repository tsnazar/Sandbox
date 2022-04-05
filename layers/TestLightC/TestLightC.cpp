#include "TestLightC.h"
#include "Core.h"
#include "Application.h"

#include <imgui.h>

#include <GLFW/glfw3.h>


TestLightC::TestLightC(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
	m_Camera.GetCamera().SetPosition({ 0, 0, 3 });
}

TestLightC::~TestLightC()
{}

void TestLightC::OnAttach()
{
	m_ObjectShader = std::make_unique<Shader>("layers/TestLightC/res/bLightVertC.glsl", "layers/TestLightC/res/bLightFragC.glsl");

	m_Material.diffuse = std::make_unique<Texture>("res/textures/container2.png");
	m_Material.specular = std::make_unique<Texture>("res/textures/container2_specular.png");
	m_Material.shininess = 64;

	GLCall(glEnable(GL_DEPTH_TEST));
}

void TestLightC::OnDetach()
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void TestLightC::OnUpdate(float dt)
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

	{	
		m_ObjectShader->Bind();
		m_ObjectShader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
		m_ObjectShader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());

		m_ObjectShader->SetUniform3f("u_Light.ambient", m_Light.ambient);
		m_ObjectShader->SetUniform3f("u_Light.diffuse", m_Light.diffuse);
		m_ObjectShader->SetUniform3f("u_Light.specular", m_Light.specular);
		m_ObjectShader->SetUniform1f("u_Light.constant", m_Light.constant);
		m_ObjectShader->SetUniform1f("u_Light.linear", m_Light.linear);
		m_ObjectShader->SetUniform1f("u_Light.quadratic", m_Light.quadratic);
		m_ObjectShader->SetUniform1f("u_Light.cutOff", glm::cos(glm::radians(m_Light.cutOff)));
		m_ObjectShader->SetUniform1f("u_Light.outerCutOff", glm::cos(glm::radians(m_Light.outerCutOff)));

		m_ObjectShader->SetUniform1i("u_Material.diffuse", 0);
		m_Material.diffuse->Bind(0);
		m_ObjectShader->SetUniform1i("u_Material.specular", 1);
		m_Material.specular->Bind(1);
		m_ObjectShader->SetUniform1f("u_Material.shininess", m_Material.shininess);

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
}

void TestLightC::OnEvent(Event& event)
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

void TestLightC::OnImGuiRender()
{
	ImGui::Text("Press F to enable/disable camera view");
	ImGui::SliderFloat("Constant attenuation", &m_Light.constant, 0.0f, 1.0f);
	ImGui::SliderFloat("Linear attenuation", &m_Light.linear, 0.0f, 1.0f);
	ImGui::SliderFloat("Quadratic attenuation", &m_Light.quadratic, 0.0f, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestLightC::RenderCube()
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
