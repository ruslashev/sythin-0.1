#include "shaders.hpp"

void Shader::Construct(std::string n_src_filename, GLuint type)
{
	_src_filename = n_src_filename;
	_debug_shader_type_str =
				type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";

	id = glCreateShader(type);

	source();

	compile();
}

void Shader::source()
{
	File src_file = readFile(_src_filename);

	const GLchar *c_str = src_file.contents.c_str();

	glShaderSource(id, 1, &c_str, NULL);
}

void Shader::compile()
{
	glCompileShader(id);

	GLint status = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[1024];
		glGetShaderInfoLog(id, 1024, NULL, buffer);

		std::string buf_str(buffer);
		// strip trailing newline
		buf_str.pop_back();
		// indent every line
		int indent = 5;
		buf_str.insert(buf_str.begin(), indent, ' ');
		for (size_t i = 0; i < buf_str.size(); i++) {
			if (buf_str[i] != '\n')
				continue;
			buf_str.insert(i, indent, ' ');
		}

		errorf("Failed to compile %s shader (\"%s\"):\n%s",
				_debug_shader_type_str.c_str(),
				_src_filename.c_str(),
				buf_str.c_str());
	}
}

Shader::~Shader()
{
	glDeleteShader(id);
}

void ShaderProgram::Construct(const Shader *vert, const Shader *frag)
{
	id = glCreateProgram();
	link(vert, frag);

	UseThisProgram();
	glBindFragDataLocation(id, 0, "out_color");

	texture_location = glGetUniformLocation(id, "gui_texture");
	glUniform1i(texture_location, 0);
	proj_mat_unif = glGetUniformLocation(id, "projection");
}

void ShaderProgram::link(const Shader *vert, const Shader *frag)
{
	glAttachShader(id, vert->id);
	glAttachShader(id, frag->id);

	glLinkProgram(id);

	GLint status = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[1024];
		glGetProgramInfoLog(id, 1024, NULL, buffer);

		std::string buf_str(buffer);
		// strip trailing newline
		buf_str.pop_back();
		// indent every line
		int indent = 5;
		buf_str.insert(buf_str.begin(), indent, ' ');
		for (size_t i = 0; i < buf_str.size(); i++) {
			if (buf_str[i] != '\n')
				continue;
			buf_str.insert(i, indent, ' ');
		}

		glDetachShader(id, vert->id);
		glDetachShader(id, frag->id);

		errorf("Failed to link shaders:\n%s",
				buf_str.c_str());
	}
}

void ShaderProgram::BindAttribute(const ArrayBuffer *buffer, const char *name,
		GLint size, GLenum type, GLboolean normalized, GLsizei stride,
		const GLvoid *ptr)
{
	buffer->Bind();
	GLint attr = glGetAttribLocation(id, name);
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, size, type, normalized, stride, ptr);
	buffer->Unbind();
}

void ShaderProgram::UseThisProgram()
{
	glUseProgram(id);
}

void ShaderProgram::DontUseThisProgram()
{
	glUseProgram(0);
}

void ShaderProgram::UpdateMatrix(const glm::mat4 &proj)
{
	glUniformMatrix4fv(proj_mat_unif, 1, GL_FALSE, glm::value_ptr(proj));
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

