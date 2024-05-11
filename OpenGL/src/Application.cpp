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
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f, // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.push<float>(2);
		va.add_buffer(vb, layout);

		IndexBuffer ib(indices, 3 * 2);

		Shader shader("res/shaders/Basic.shader");

		shader.unbind();
		va.unbind();
		vb.unbind();
		ib.unbind();

		float r = 0.0f;
		float increment = 0.05f;

		const double fpsLimit = 1.0 / 30.0;
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

			if ((now - lastFrameTime) >= fpsLimit)
			{
				// Render here
				GLCall(glClear(GL_COLOR_BUFFER_BIT));

				shader.bind();
				shader.set_uniform_4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				va.bind();
				ib.bind();
				GLCall(glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, nullptr));

				if (r > 1.0f)
					increment = -0.05f;
				else if (r < 0.0f)
					increment = 0.05f;
				r += increment;

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
