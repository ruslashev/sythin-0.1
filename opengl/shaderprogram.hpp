#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "arraybuffer.hpp"

class ShaderProgram
{
	void link(const Shader *vert, const Shader *frag);
public:
	GLuint id;
	GLint proj_mat_unif, texture_location;

	~ShaderProgram();

	void Construct(const Shader *vert, const Shader *frag);
	void BindAttribute(const ArrayBuffer *buffer, const char *name,
		GLint size, GLenum type, GLboolean normalized, GLsizei stride,
		const GLvoid *ptr);
	void UseThisProgram();
	void DontUseThisProgram();
	void UpdateMatrix(const glm::mat4 &proj);
};

#endif

