#include "arraybuffer.hpp"

void ArrayBuffer::Construct()
{
	glGenBuffers(1, &_id);
}

void ArrayBuffer::Upload(std::vector<GLfloat> &data)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER,
			data.size()*sizeof(data[0]),
			data.data(),
			GL_STATIC_DRAW);
	Unbind();
}

void ArrayBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void ArrayBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ArrayBuffer::~ArrayBuffer()
{
	glDeleteBuffers(1, &_id);
}

