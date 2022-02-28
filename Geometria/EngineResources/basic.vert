#version 130

in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_uv;
in float textureIndex;

out vec3 vs_position;
out vec4 vs_color;
out vec2 vs_uv;
flat out int texIndex;

uniform mat4 MVP;

void main()
{
	vs_color = vertex_color;
	vs_uv = vec2(vertex_uv.x, vertex_uv.y);
	texIndex = int(textureIndex);

	vs_position = vertex_position;
	gl_Position = MVP * vec4(vs_position, 1.f);
}
