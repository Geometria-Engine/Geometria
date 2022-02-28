#include <GL/glew.h>
#include <glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../../Externals/Model.h"

class Shader
{
private:
	// Stores all possible plain text or information useful here.
	// List:
	// - data[0] = GLSL Shader (OpenGL).

	const char* data[12];

	//Stores all IDs or numbers that can link to other parts of the engine.
	// List:
	// - ids[0] = Shader ID (OpenGL).

	int ids[12];

	// Checks if the compilation resulted in an error.
	bool compilationFailed = false;

public:
	Shader()
	{
	}

#pragma region OpenGL
	//==[ OPENGL ]==//
	const char*& OpenGLVertexShader() { return data[0]; }
	const char*& OpenGLFragmentShader() { return data[1]; }
	int& OpenGLShaderID() { return ids[0]; }

private:
	GLuint OpenGLProgram;
	GLuint OGL_VertexShader, OGL_FragmentShader;
	GLint OGL_CompilerResult;

	std::unordered_map<std::string, GLint> OGL_uniformLocationCache;
public:

	Shader(std::string vertexContent, std::string fragmentContent);
	void ReportShaderError(GLuint& shader, const char* title);

	GLint OGL_GetUniformLocation(const GLchar* name);
	//==[ OPENGL ]==//
#pragma endregion

	void GPU_SetFloat(const char* name, float value);
	void GPU_SetInt(const char* name, int value);
	void GPU_SetIntArray(const char* name, int* value, int size);

	void GPU_SetVector2(const char* name, const Vector2 vector);
	void GPU_SetVector3(const char* name, const Vector3 vector);
	void GPU_SetVector4(const char* name, const Vector4 vector);
	void GPU_SetMatrix(const char* name, const Matrix& matrix);

	void Enable() const;
	void Disable() const;

	void ClearFromRAM();

	int LoadShader();
private:
	void Start();
	void Compilation();
	void FinalTouches();
};
