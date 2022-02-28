#include "Shader.h"
#include <iostream>

Shader::Shader(std::string vertexContent, std::string fragmentContent)
{
	//==[ OPENGL ]==//
	{
		OpenGLVertexShader() = &vertexContent[0];
		OpenGLFragmentShader() = &fragmentContent[0];

		OpenGLShaderID() = LoadShader();
	}
}

int Shader::LoadShader()
{
	compilationFailed = false;

	//The process of loading a shaders has 3 steps or stages.
	//This is ordered like so, in order to make it easier to follow what it does.

	Start();
	Compilation();

	if(!compilationFailed)
		FinalTouches();

	//==[ OPENGL ]==//
	return OpenGLProgram;
}

void Shader::Start()
{
	//==[ OPENGL ]==//
	{
		OpenGLProgram = glCreateProgram();
		OGL_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		OGL_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	}
}

void Shader::Compilation()
{
	//==[ OPENGL ]==//
	{
		//Compile vertex shader
		glShaderSource(OGL_VertexShader, 1, &OpenGLVertexShader(), NULL);
		glCompileShader(OGL_VertexShader);
		glGetShaderiv(OGL_VertexShader, GL_COMPILE_STATUS, &OGL_CompilerResult);

		if (OGL_CompilerResult == GL_FALSE)
		{
			ReportShaderError(OGL_VertexShader, "VertexShader");
			compilationFailed = true;
			return;
		}

		//Compile fragment shader
		glShaderSource(OGL_FragmentShader, 1, &OpenGLFragmentShader(), NULL);
		glCompileShader(OGL_FragmentShader);

		glGetShaderiv(OGL_FragmentShader, GL_COMPILE_STATUS, &OGL_CompilerResult);

		if (OGL_CompilerResult == GL_FALSE)
		{
			ReportShaderError(OGL_FragmentShader, "FragmentShader");
			compilationFailed = true;
			return;
		}
	}
}

void Shader::FinalTouches()
{
	//==[ OPENGL ]==//
	{
		glAttachShader(OpenGLProgram, OGL_VertexShader);
		glAttachShader(OpenGLProgram, OGL_FragmentShader);

		glLinkProgram(OpenGLProgram);
		glValidateProgram(OpenGLProgram);

		glDeleteShader(OGL_VertexShader);
		glDeleteShader(OGL_FragmentShader);

		std::cout << "Shader successfully compiled!" << std::endl;
	}
}

void Shader::ReportShaderError(GLuint& shader, const char* title)
{
	//==[ OPENGL ]==//
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(shader, length, &length, &error[0]);

		std::cout << "Error compiling shader (" << title << ") >> " << &error[0] << std::endl;
		glDeleteShader(shader);
	}
}

//====================[ OPENGL Only ]========================//
GLint Shader::OGL_GetUniformLocation(const GLchar* name)
{
	if (OGL_uniformLocationCache.find(name) != OGL_uniformLocationCache.end())
		return OGL_uniformLocationCache[name];

	GLint location = glGetUniformLocation(OpenGLShaderID(), name);
	OGL_uniformLocationCache[name] = location;
	return location;
}
//====================[ OPENGL Only ]========================//

void Shader::GPU_SetFloat(const char* name, float value)
{
	//==[ OPENGL ]==//
	{
		glUniform1f(OGL_GetUniformLocation(name), value);
	}
}

void Shader::GPU_SetInt(const char* name, int value)
{
	//==[ OPENGL ]==//
	{
		glUniform1i(OGL_GetUniformLocation(name), value);
	}
}

void Shader::GPU_SetIntArray(const char* name, int* value, int size)
{
	//==[ OPENGL ]==//
	{
		glUniform1iv(OGL_GetUniformLocation(name), size, value);
	}
}

void Shader::GPU_SetVector2(const char* name, const Vector2 vector)
{
	//==[ OPENGL ]==//
	{
		glUniform2f(OGL_GetUniformLocation(name), vector.x, vector.y);
	}
}

void Shader::GPU_SetVector3(const char* name, const Vector3 vector)
{
	//==[ OPENGL ]==//
	{
		glUniform3f(OGL_GetUniformLocation(name), vector.x, vector.y, vector.z);
	}
}

void Shader::GPU_SetVector4(const char* name, const Vector4 vector)
{
	//==[ OPENGL ]==//
	{
		glUniform4f(OGL_GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}
}

void Shader::GPU_SetMatrix(const char* name, const Matrix& matrix)
{
	//==[ OPENGL ]==//
	{
		glUniformMatrix4fv(OGL_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix.coreMatrix));
	}
}

void Shader::Enable() const
{
	//==[ OPENGL ]==//
	{
		glUseProgram(OpenGLProgram);
	}
}

void Shader::Disable() const
{
	//==[ OPENGL ]==//
	{
		glUseProgram(0);
	}
}

void Shader::ClearFromRAM()
{
	//Yeah, every bit of data needs to be vanished from this earth once i call this

	OGL_uniformLocationCache.clear();
	std::unordered_map<std::string, GLint>().swap(OGL_uniformLocationCache);
}
