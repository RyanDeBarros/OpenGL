#include "TestSlideshow.h"

#include "imgui/imgui.h"

#include "Renderer.h"
#include "Application.h"

namespace test {

	TestSlideshow::TestSlideshow(std::vector<std::pair<std::string, std::string>> shaders)
		: m_ShaderIndex(0), m_TotalTime(0), m_Scale{ 0.6, 0.8 }
	{
		ASSERT(!shaders.empty())
		for (auto shader : shaders)
			m_Shaders.push_back(std::make_pair(new Shader("res/shaders/Slide.vert", shader.first), shader.second));
		m_CurrentShader = m_Shaders[0].first;

		float vertices[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f,
		};
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 12 * sizeof(float));
		VertexBufferLayout layout;
		layout.push<float>(2);
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->add_buffer(*m_VertexBuffer, layout);

		unsigned int indeces[] = {
			0, 1, 2,
			2, 3, 0
		};
		m_IndexBuffer = std::make_unique<IndexBuffer>(indeces, 6);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	
	TestSlideshow::~TestSlideshow()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		for (auto shader : m_Shaders)
			delete shader.first;
	}
	
	void TestSlideshow::OnUpdate(double deltaTime)
	{
		m_TotalTime += deltaTime;
		m_CurrentShader->bind();
		m_CurrentShader->set_uniform_2f("u_Scale", m_Scale[0], m_Scale[1]);
		m_CurrentShader->set_uniform_1d("u_DeltaTime", deltaTime);
		m_CurrentShader->set_uniform_1d("u_TotalTime", m_TotalTime);
	}
	
	void TestSlideshow::OnRender()
	{
		renderer.draw(*m_VAO, *m_IndexBuffer, *m_CurrentShader);
	}
	
	void TestSlideshow::OnImGuiRender()
	{
		ImGui::Text(m_Shaders[m_ShaderIndex].second.c_str());
		if (ImGui::Button("NEXT"))
		{
			m_ShaderIndex = (m_ShaderIndex + 1) % m_Shaders.size();
			m_CurrentShader = m_Shaders[m_ShaderIndex].first;
			m_TotalTime = 0.0;
		}
		else if (ImGui::Button("PREV"))
		{
			m_ShaderIndex = (m_ShaderIndex - 1) % m_Shaders.size();
			m_CurrentShader = m_Shaders[m_ShaderIndex].first;
			m_TotalTime = 0.0;
		}
		else if (ImGui::Button("R"))
		{
			m_TotalTime = 0.0;
		}
		ImGui::SliderFloat2("Scale", m_Scale, 0.01, 2.0);
	}

}
