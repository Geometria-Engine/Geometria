precision highp float;
precision highp int;

in vec3 vertex_position;
in vec4 vertex_color;

in float miniShaderId;

in vec2 vertex_uv;
in float textureIndex;

//in vec2 second_vertex_uv;
//in float secondTextureIndex;
//
//in vec2 third_vertex_uv;
//in float thirdTextureIndex;
//
//in vec2 forth_vertex_uv;
//in float forthTextureIndex;
//
//in vec2 fifth_vertex_uv;
//in float fifthTextureIndex;
//
//in vec2 sixth_vertex_uv;
//in float sixthTextureIndex;
//
//in vec2 seventh_vertex_uv;
//in float seventhTextureIndex;
//
//in vec2 eighth_vertex_uv;
//in float eighthTextureIndex;
//
//in vec2 ninth_vertex_uv;
//in float ninthTextureIndex;
//
//in vec2 tenth_vertex_uv;
//in float tenthTextureIndex;

out vec3 vs_position;
out vec4 vs_color;
out vec2 FirstTexture;
flat out float texIndex;
flat out float vs_miniShaderId;

uniform mat4 MVP;

void main()
{
	vs_color = vertex_color;
	FirstTexture = vec2(vertex_uv.x, vertex_uv.y);
	texIndex = textureIndex;

	vs_miniShaderId = miniShaderId;
	int Master_MiniShaderID = int(miniShaderId);

	{ApplyMiniShaders}

	vs_position = vs_position;
	gl_Position = MVP * vec4(vertex_position, 1.0f);
}
