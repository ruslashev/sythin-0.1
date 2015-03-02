#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include <GL/glew.h>

class VertexArray
{
public:
	GLuint id;

	~VertexArray();

	void Construct();
};

#endif

