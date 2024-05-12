#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main()
{
	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const float windowWidth = 1440.0f, windowHeight = 1080.0f;

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow((int)windowWidth, (int)windowHeight, "Hello World!", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND))
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] = {
			200.0f, 200.0f, 0.0f, 0.0f, // 0
			400.0f, 200.0f, 1.0f, 0.0f, // 1
			400.0f, 400.0f, 1.0f, 1.0f, // 2
			200.0f, 400.0f, 0.0f, 1.0f // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		va.add_buffer(vb, layout);

		IndexBuffer ib(indices, 3 * 2);
		
		glm::mat4 proj = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0, 0.0, 0.0));

		Shader shader("res/shaders/Basic.shader");
		shader.bind();

		Texture texture("res/textures/TheChernoLogo.png");
		const int slot = 0;
		texture.bind(slot);
		shader.set_uniform_1i("u_Texture", slot);

		shader.unbind();
		va.unbind();
		vb.unbind();
		ib.unbind();
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init(); 
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		const double fpsLimit = 1.0 / 60.0;
		double lastUpdateTime = 0;	// number of seconds since the last loop
		double lastFrameTime = 0;	// number of seconds since the last frame

		glm::vec3 translation(200.0, 200.0, 0.0);

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			double now = glfwGetTime();
			double deltaTime = now - lastUpdateTime;

			// Poll for and process events
			glfwPollEvents();

			// Application logic here, using deltaTime if necessary

			if ((now - lastFrameTime) >= fpsLimit)
			{
				renderer.clear();
				
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				
				// Render here
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
				glm::mat4 mvp = proj * view * model;

				shader.bind();
				shader.set_uniform_mat4f("u_MVP", mvp);
				renderer.draw(va, ib, shader);

				{
					ImGui::Begin("Model View Projection");
					ImGui::SetWindowFontScale(2.0f);
					ImGui::SliderFloat3("Translation", &translation.x, 0.0f, windowWidth);
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
					ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap front and back buffers
				glfwSwapBuffers(window);

				// Set last frame update
				lastFrameTime = now;
			}

			lastUpdateTime = now;
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
