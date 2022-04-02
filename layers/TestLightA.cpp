#include "TestLightA.h"
#include "Utility.h"
#include "Application.h"

#include <imgui.h>

#include <GLFW/glfw3.h>


TestLightA::TestLightA(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
	m_Camera.GetCamera().SetPosition({ 0, 0, 3 });
}

TestLightA::~TestLightA()
{}

void TestLightA::OnAttach()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,	0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f,

	 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	GLCall(glEnable(GL_DEPTH_TEST));

	m_ObjectShader = std::make_unique<Shader>("res/shaders/bLightVertA.glsl", "res/shaders/bLightFragA.glsl");
	m_LightShader = std::make_unique<Shader>("res/shaders/basicVertex.glsl", "res/shaders/basicFragment.glsl");

	GLCall(glCreateVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	GLCall(glCreateBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)12));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)24));

	GLCall(glCreateBuffers(1, &m_IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	m_Material.diffuse = std::make_unique<Texture>("res/textures/container2.png");
	m_Material.specular = std::make_unique<Texture>("res/textures/container2_specular.png");
	m_Material.shininess = 64;
}

void TestLightA::OnDetach()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDeleteVertexArrays(1, &m_VAO));
	GLCall(glDeleteBuffers(1, &m_VBO));
	GLCall(glDeleteBuffers(1, &m_IBO));
}

void TestLightA::OnUpdate(float dt)
{
	if(m_CameraEnabled)
		m_Camera.OnUpdate(dt);

	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	{
		glm::mat4 MVP = m_Camera.GetCamera().GetViewProjectionMatrix();
		glm::mat4 model =  glm::mat4(1.0f);

		m_ObjectShader->Bind();
		m_ObjectShader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
		m_ObjectShader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());
		m_ObjectShader->SetUniformMat4f("u_Model", model);

		m_ObjectShader->SetUniform3f("u_LightPos", m_Light.lightPos);

		m_ObjectShader->SetUniform3f("u_Light.ambient", m_Light.ambient);
		m_ObjectShader->SetUniform3f("u_Light.diffuse", m_Light.diffuse);
		m_ObjectShader->SetUniform3f("u_Light.specular", m_Light.specular);

		m_ObjectShader->SetUniform1i("u_Material.diffuse", 0);
		m_Material.diffuse->Bind(0);
		m_ObjectShader->SetUniform1i("u_Material.specular", 1);
		m_Material.specular->Bind(1);
		m_ObjectShader->SetUniform1f("u_Material.shininess", m_Material.shininess);

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
	}

	{
		m_Light.lightPos.x = 1 + sin(glfwGetTime()) * 2.f;
		m_Light.lightPos.y = sin(glfwGetTime() / 2);
		glm::mat4 MVP = m_Camera.GetCamera().GetViewProjectionMatrix() * glm::translate(glm::mat4(1.0f), m_Light.lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("u_MVP", MVP);
		m_LightShader->SetUniform4f("u_Color", glm::vec4(m_Light.specular, 1.0f));

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
	}
}

void TestLightA::OnEvent(Event& event)
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

void TestLightA::OnImGuiRender()
{
	ImGui::Text("Press F to enable/disable camera view");
	ImGui::ColorEdit3("Light Color Ambient", glm::value_ptr(m_Light.ambient));
	ImGui::ColorEdit3("Light Color Diffuse", glm::value_ptr(m_Light.diffuse));
	ImGui::ColorEdit3("Light Color Specular", glm::value_ptr(m_Light.specular));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
