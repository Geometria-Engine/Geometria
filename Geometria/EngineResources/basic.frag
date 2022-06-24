precision mediump float;

in vec3 vs_position;
in vec4 vs_color;
in vec2 FirstTexture;
flat in float texIndex;
flat in float vs_miniShaderId;

uniform sampler2D textures0;
uniform sampler2D textures1;
uniform sampler2D textures2;
uniform sampler2D textures3;
uniform sampler2D textures4;
uniform sampler2D textures5;
uniform sampler2D textures6;
uniform sampler2D textures7;
uniform sampler2D textures8;
uniform sampler2D textures9;
uniform sampler2D textures10;
uniform sampler2D textures11;
uniform sampler2D textures12;
uniform sampler2D textures13;
uniform sampler2D textures14;
uniform sampler2D textures15;

out vec4 fs_color;

void Master_UseFirstTexture(inout vec4 t)
{
    int index = int(texIndex);

    switch(index)
    {
    case -1:
        t = vec4(1, 1, 1, 1);
        break;
    case 0:
        t = texture2D(textures0, FirstTexture);
        break;
    case 1:
        t = texture2D(textures1, FirstTexture);
        break;
    case 2:
        t = texture2D(textures2, FirstTexture);
        break;
    case 3:
        t = texture2D(textures3, FirstTexture);
        break;
    case 4:
        t = texture2D(textures4, FirstTexture);
        break;
    case 5:
        t = texture2D(textures5, FirstTexture);
        break;
    case 6:
        t = texture2D(textures6, FirstTexture);
        break;
    case 7:
        t = texture2D(textures7, FirstTexture);
        break;
    case 8:
        t = texture2D(textures8, FirstTexture);
        break;
    case 9:
        t = texture2D(textures9, FirstTexture);
        break;
    case 10:
        t = texture2D(textures10, FirstTexture);
        break;
    case 11:
        t = texture2D(textures11, FirstTexture);
        break;
    case 12:
        t = texture2D(textures12, FirstTexture);
        break;
    case 13:
        t = texture2D(textures13, FirstTexture);
        break;
    case 14:
        t = texture2D(textures14, FirstTexture);
        break;
    case 15:
        t = texture2D(textures15, FirstTexture);
        break;
    }
}

void main()
{
    int Master_MiniShaderID = int(vs_miniShaderId);
    vec4 f_tex = vec4(1, 1, 1, 1);
    Master_UseFirstTexture(f_tex);
    vec3 Master_VertPosition = vs_position;
    vec4 Master_Color = vs_color * f_tex;

    if(Master_Color.a < 0.1)
    {
        discard;
    }

    {ApplyMiniShaders}

    fs_color = Master_Color;
}
