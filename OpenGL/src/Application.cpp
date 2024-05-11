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

int main()
{
	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1440, 1080, "Hello World!", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND))
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 1.0f // 3
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

		float r = 0.0f;
		float increment = 3.0f;

		const double fpsLimit = 1.0 / 60.0;
		double lastUpdateTime = 0;	// number of seconds since the last loop
		double lastFrameTime = 0;	// number of seconds since the last frame

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			double now = glfwGetTime();
			double deltaTime = now - lastUpdateTime;

			// Poll for and process events
			glfwPollEvents();

			// Application logic here, using deltaTime if necessary
			if (r > 1.0f || r < 0.0f)
				increment *= -1;
			r += increment * deltaTime;

			if ((now - lastFrameTime) >= fpsLimit)
			{
				// Render here
				renderer.clear();

				shader.bind();
				shader.set_uniform_4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				
				renderer.draw(va, ib, shader);

				// Swap front and back buffers
				glfwSwapBuffers(window);

				// Set last frame update
				lastFrameTime = now;
			}

			lastUpdateTime = now;
		}
	}

	glfwTerminate();
	return 0;
}
