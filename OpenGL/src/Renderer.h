#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCall(x) glClearError(); x; ASSERT(glLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void clear() const;
	void draw(const class VertexArray& va, const class IndexBuffer& ib, const class Shader& shader) const;
};
