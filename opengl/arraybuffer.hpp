#ifndef ARRAYBUFFER_HPP
#define ARRAYBUFFER_HPP

#include <GL/glew.h>

#include <vector>

class ArrayBuffer
{
	GLuint _id;
public:
	~ArrayBuffer();

	void Construct();
	void Upload(std::vector<GLfloat> &data);
	void Bind() const;
	void Unbind() const;
};

#endif

