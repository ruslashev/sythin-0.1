#version 130

uniform mat4 projection;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main()
{
	frag_uv = uv;
	frag_color = color;

	gl_Position = projection*vec4(position, 0.0, 1.0);
}

