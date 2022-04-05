#include "CornellBox.h"
#include "Core.h"
#include "Application.h"

#include <imgui.h>

#include <GLFW/glfw3.h>


CornellBox::CornellBox(const std::string& name /*= "Layer"*/)
	: m_Camera(90.0f, 16.0f / 9.0f, 1.0f, 1600.f)
{
	m_Camera.GetCamera().SetPosition({ 278, 273, -800 });
	m_Camera.GetCamera().SetAngles(90, 0);
	m_Camera.SetVelocity(200.f);
}

CornellBox::~CornellBox()
{}

void CornellBox::OnAttach()
{
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_FRAMEBUFFER_SRGB));

	m_ObjectShader = std::make_unique<Shader>("layers/CornellBox/res/corBoxVert.glsl", "layers/CornellBox/res/corBoxFrag.glsl");
	m_LightShader = std::make_unique<Shader>("layers/CornellBox/res/basicVertex.glsl", "layers/CornellBox/res/basicFragment.glsl");
	m_ShadowDepthShader = std::make_unique<Shader>("layers/CornellBox/res/shadowDepthVert.glsl", "layers/CornellBox/res/shadowDepthFrag.glsl");

	m_ScreenShader = std::make_unique<Shader>("layers/ShadowMap/res/shadowDebugVert.glsl", "layers/ShadowMap/res/shadowDebugFrag.glsl");

	m_ShadowMapFBO = std::make_unique<ShadowMapFBO>(ShadowMapType::TEXTURE2D, 1024, 1024);
}

void CornellBox::OnDetach()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDisable(GL_FRAMEBUFFER_SRGB));
}

void CornellBox::OnUpdate(float dt)
{
	if(m_CameraEnabled)
		m_Camera.OnUpdate(dt);


	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 200.0f, far_plane = 600.0f;
	lightProjection = glm::perspective(glm::radians(100.0f), 1.0f, near_plane, far_plane);
	lightView = glm::lookAt(m_Light.Position, m_Light.Position + glm::vec3(0, -1, 0), glm::vec3(-1.0, 0.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	m_ShadowMapFBO->Bind();
	m_ShadowDepthShader->Bind();
	m_ShadowDepthShader->SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMatrix);
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	RenderScene();
	m_ShadowMapFBO->Unbind();

	/*GLCall(glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	m_ScreenShader->Bind();
	m_ScreenShader->SetUniform1i("u_DepthMap", 0);
	m_ShadowMapFBO->BindTexture(0);
	m_ScreenShader->SetUniform1f("u_FarPlane", far_plane);
	m_ScreenShader->SetUniform1f("u_NearPlane", near_plane);
	RenderQuad();*/
	
	GLCall(glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_ObjectShader->Bind();
	m_ObjectShader->SetUniformMat4f("u_Projection", m_Camera.GetCamera().GetProjectionMatrix());
	m_ObjectShader->SetUniformMat4f("u_View", m_Camera.GetCamera().GetViewMatrix());
	m_ObjectShader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
	m_ObjectShader->SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMatrix);
	m_ObjectShader->SetUniform1i("u_DepthMap", 0);
	m_ShadowMapFBO->BindDepthTexture(0);

	m_ObjectShader->SetUniform1i("u_Blinn", m_Blinn);

	m_ObjectShader->SetUniform3f("u_Light.Position", m_Light.Position);
	m_ObjectShader->SetUniform3f("u_Light.Ambient", m_Light.Ambient);
	m_ObjectShader->SetUniform3f("u_Light.Diffuse", m_Light.Diffuse);
	m_ObjectShader->SetUniform3f("u_Light.Specular", m_Light.Specular);
	m_ObjectShader->SetUniform1f("u_Light.Constant", m_Light.Constant);
	m_ObjectShader->SetUniform1f("u_Light.Linear", m_Light.Linear);
	m_ObjectShader->SetUniform1f("u_Light.Quadratic", m_Light.Quadratic);
	RenderScene();
}

void CornellBox::OnEvent(Event& event)
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

void CornellBox::OnImGuiRender()
{
	ImGui::Checkbox("Blinn", &m_Blinn);
	ImGui::Text("Press F to enable/disable camera view");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void CornellBox::RenderScene()
{
	if (m_VertexArray == nullptr)
	{
		float vertices[] = {
			//floor
			552.8, 0.0,   0.0,      1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.0,   0.0,   0.0,	    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.0,   0.0,   559.2,    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			549.6, 0.0,   559.2,    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			//ceil	   								  
			556.0, 548.8, 0.0,	    1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			556.0, 548.8, 559.2,    1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			0.0,   548.8, 559.2,    1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			0.0,   548.8, 0.0,	    1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			//back wall	   		    				  
			549.6, 0.0  , 559.2,    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			0.0  , 0.0  , 559.2,    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			0.0  , 548.8, 559.2,    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			556.0, 548.8, 559.2,    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			//right wall		    				  
			0.0,   0.0,   559.2,    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.f,
			0.0,   0.0,   0.0,	    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.f,
			0.0,   548.8, 0.0,	    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.f,
			0.0,   548.8, 559.2,    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.f,
			//left wall			    				  
			552.8, 0.0,   0.0,	    1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.f,
			549.6, 0.0,   559.2,    1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.f,
			556.0, 548.8, 559.2,    1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.f,
			556.0, 548.8,  0.0,	    1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.f,
			//short block							  
			130.0, 165.0,  65.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 82.0, 165.0, 225.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			240.0, 165.0, 272.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			290.0, 165.0, 114.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			290.0,   0.0, 114.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			290.0, 165.0, 114.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			240.0, 165.0, 272.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			240.0,   0.0, 272.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

			130.0,   0.0,  65.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			130.0, 165.0,  65.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			290.0, 165.0, 114.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			290.0,   0.0, 114.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

			82.0,   0.0, 225.0,		1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			 82.0, 165.0, 225.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			130.0, 165.0,  65.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			130.0,   0.0,  65.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,

			240.0,   0.0, 272.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			240.0, 165.0, 272.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 82.0, 165.0, 225.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 82.0,   0.0, 225.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 //tall block			
			 423.0, 330.0, 247.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 265.0, 330.0, 296.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 314.0, 330.0, 456.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 472.0, 330.0, 406.0,	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			 423.0,   0.0, 247.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 423.0, 330.0, 247.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 472.0, 330.0, 406.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 472.0,   0.0, 406.0,	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

			 472.0,   0.0, 406.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 472.0, 330.0, 406.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 314.0, 330.0, 456.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 314.0,   0.0, 456.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			 314.0,   0.0, 456.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			 314.0, 330.0, 456.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			 265.0, 330.0, 296.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,
			 265.0,   0.0, 296.0,	1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f,

			 265.0,   0.0, 296.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 265.0, 330.0, 296.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 423.0, 330.0, 247.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 423.0,   0.0, 247.0,	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		};

		unsigned int indices[17 * 6];

		for (unsigned int i = 0; i < 17; i++)
		{
			indices[6 * i] = 0 + i * 4;
			indices[6 * i + 1] = 1 + i * 4;
			indices[6 * i + 2] = 2 + i * 4;
			indices[6 * i + 3] = 2 + i * 4;
			indices[6 * i + 4] = 3 + i * 4;
			indices[6 * i + 5] = 0 + i * 4;
		}

		m_VertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout = {
			{VertexBufferElementType::Float3, "a_Position"},
			{VertexBufferElementType::Float3, "a_Color"},
			{VertexBufferElementType::Float3, "a_Normal"},
		};

		vertexBuffer->SetLayout(layout);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
	//render
	m_VertexArray->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
}

void CornellBox::RenderQuad()
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