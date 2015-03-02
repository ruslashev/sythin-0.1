#version 130

uniform sampler2D gui_texture;

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

void main()
{
	out_color = frag_color * texture(gui_texture, frag_uv.st);
}

