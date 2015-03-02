#include "shader.hpp"

void Shader::Construct(std::string n_src_filename, GLuint n_type)
{
	_src_filename = n_src_filename;
	_type = n_type;
	_debug_shader_type_str =
				_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";

	id = glCreateShader(_type);

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

