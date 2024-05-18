#include "TestTexture2D.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "Application.h"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
		float positions[] = {
			-100.0f, -100.0f, 0.0f, 0.0f, // 0
			 100.0f, -100.0f, 1.0f, 0.0f, // 1
			 100.0f,  100.0f, 1.0f, 1.0f, // 2
			-100.0f,  100.0f, 0.0f, 1.0f  // 3
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		m_VAO->add_buffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->bind();
		m_Shader->set_uniform_4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		m_Texture = std::make_unique<Texture>("res/textures/TheChernoLogo.png");
		m_Shader->set_uniform_1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	
	void TestTexture2D::OnUpdate(double deltaTime)
	{
	}
	
	void TestTexture2D::OnRender()
	{
		m_Texture->bind();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->bind();
			m_Shader->set_uniform_mat4f("u_MVP", mvp);
			renderer.draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->bind();
			m_Shader->set_uniform_mat4f("u_MVP", mvp);
			renderer.draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}
	
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, windowWidth);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, windowWidth);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
