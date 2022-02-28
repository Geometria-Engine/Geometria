#version 130

in vec4 vs_color;
in vec2 vs_uv;
flat in int texIndex;

uniform sampler2D textures0;
uniform sampler2D textures1;

out vec4 fs_color;

void main()
{
    switch(texIndex)
    {
    case -1:
        fs_color = vs_color;
        break;
    case 0:
        fs_color = vs_color * texture(textures0, vs_uv);
        break;
    case 1:
        fs_color = vs_color * texture(textures1, vs_uv);
        break;
    }
}
