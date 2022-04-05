#include "TestCube.h"
#include "Core.h"
#include "KeyEvent.h"
#include "Application.h"

#include <imgui.h>
#include "GLFW/glfw3.h"

TestCube::TestCube(const std::string& name /*= "Layer"*/)
	: m_Camera(60.0f, 16.0f / 9.0f)
{
}

TestCube::~TestCube()
{}

void TestCube::OnAttach()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
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

	m_Shader = std::make_unique<Shader>("layers/Cube/res/basicVertex.glsl", "layers/Cube/res/basicFragment.glsl");

	GLCall(glCreateVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	GLCall(glCreateBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));

	GLCall(glCreateBuffers(1, &m_IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

void TestCube::OnDetach()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDeleteVertexArrays(1, &m_VAO));
	GLCall(glDeleteBuffers(1, &m_VBO));
	GLCall(glDeleteBuffers(1, &m_IBO));
}

void TestCube::OnUpdate(float dt)
{
	if(m_CameraEnabled)
		m_Camera.OnUpdate(dt);

	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	glm::mat4 MVP = m_Camera.GetCamera().GetViewProjectionMatrix();
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", MVP);
	m_Shader->SetUniform4f("u_Color", m_SquareColor);

	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
}

void TestCube::OnEvent(Event& event)
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

void TestCube::OnImGuiRender()
{
	ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareColor));
	ImGui::Text("Press F to enable\disable camera view");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
