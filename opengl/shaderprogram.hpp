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
	GLint _proj_mat_unif, _texture_location;

	void link(const Shader *vert, const Shader *frag);
public:
	GLuint id;

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

