#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "tests/TestClearColor.h"

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const float windowWidth = 1440.0f, windowHeight = 1080.0f;

	GLFWwindow* window = glfwCreateWindow((int)windowWidth, (int)windowHeight, "Hello World!", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND))
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init(); 
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		const double fpsLimit = 1.0 / 60.0;
		double lastUpdateTime = 0;	// number of seconds since the last loop
		double lastFrameTime = 0;	// number of seconds since the last frame

		// Setup here
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");

		while (!glfwWindowShouldClose(window))
		{
			double now = glfwGetTime();
			double deltaTime = now - lastUpdateTime;
			glfwPollEvents();

			// Application logic here, using deltaTime if necessary
			if (currentTest)
				currentTest->OnUpdate(deltaTime);

			if ((now - lastFrameTime) >= fpsLimit)
			{
				renderer.clear();
				
				// Render here
				if (currentTest)
					currentTest->OnRender();

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				
				// ImGui render here
				if (currentTest)
				{
					ImGui::Begin("Test");
					ImGui::SetWindowFontScale(1.5f);
					if (currentTest != testMenu && ImGui::Button("<-"))
					{
						delete currentTest;
						currentTest = testMenu;
					}
					currentTest->OnImGuiRender();
					ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				glfwSwapBuffers(window);
				lastFrameTime = now;
			}

			lastUpdateTime = now;
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
