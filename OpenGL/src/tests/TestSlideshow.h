#pragma once

#include "Test.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace test {

	class TestSlideshow : public Test
	{
	public:
		TestSlideshow(std::vector<std::pair<std::string, std::string>> shaders);
		~TestSlideshow();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		int m_ShaderIndex;
		std::vector<std::pair<Shader*, std::string>> m_Shaders;
		Shader* m_CurrentShader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		double m_TotalTime;
		float m_Scale[2];
	};

}
