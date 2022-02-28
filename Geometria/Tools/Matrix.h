#include "Vector.h"
#include <glm/detail/type_vec4.hpp>
#include <iostream>
#include <sstream>

struct Matrix {
	glm::mat4 coreMatrix;

	Matrix() { /* C++ la neta como chingas la madre con los constructores vales verga */ }
	
	Matrix(const float& A)
	{
		coreMatrix = glm::mat4(A);
	}

	float& operator()(int x, int y)
	{
		return coreMatrix[x][y];
	}

	Matrix operator=(const Matrix& A)
	{
		coreMatrix = A.coreMatrix;
		return *this;
	}

	Matrix operator=(const float& A)
	{
		coreMatrix = glm::mat4(A);
		return *this;
	}

	Matrix operator+=(const Matrix& A)
	{
		coreMatrix += A.coreMatrix;
		return *this;
	}

	Matrix operator+(const Matrix& A) { Matrix result = *this; return result += A; }

	Matrix operator-=(const Matrix& A)
	{
		coreMatrix -= A.coreMatrix;
		return *this;
	}

	Matrix operator-(const Matrix& A) { Matrix result = *this; return result -= A; }

	Matrix operator-()
	{
		Matrix result = *this;
		result.coreMatrix = -result.coreMatrix;
		return result;
	}

	Matrix operator*=(const Matrix& A) {
		coreMatrix *= A.coreMatrix;
		return *this;
	}

	Matrix operator*(const Matrix& A) { Matrix result = *this; return result *= A; }

	Vector4 operator*(const Vector4& A)
	{
		Matrix result = *this;
		glm::vec4 m;
		m.x = A.x;
		m.y = A.y;
		m.z = A.z;
		m.w = A.w;

		m = result.coreMatrix * m;
		return Vector4(m.x, m.y, m.z, m.w);
	}

	Matrix operator/=(const float& A) {
		coreMatrix /= glm::mat4(A);
		return *this;
	}

	Matrix operator/(const float& A) { Matrix result = *this; return result /= A; }

	std::string PrintMatrixData()
	{
		std::ostringstream stream;
		for (int x = 0; x < 4; x++)
		{
			stream << "( ";
			for (int y = 0; y < 4; y++)
			{
				stream << coreMatrix[x][y] << " ";
			}
			stream << ")" << std::endl;
		}
		return stream.str();
	}

	static Matrix Translate(const Vector3& A)
	{
		Matrix m(1);
		m.coreMatrix = glm::translate(m.coreMatrix, glm::vec3(A.x, A.y, A.z));
		return m;
	}

	static Matrix Translate(Matrix& matrix, const Vector3& A)
	{
		matrix.coreMatrix = glm::translate(matrix.coreMatrix, glm::vec3(A.x, A.y, A.z));
		return matrix;
	}

	static Matrix Scale(const Vector3& A)
	{
		Matrix m(1);
		m.coreMatrix = glm::scale(m.coreMatrix, glm::vec3(A.x, A.y, A.z));
		return m;
	}

	static Matrix Scale(Matrix& matrix, const Vector3& A)
	{
		matrix.coreMatrix = glm::scale(matrix.coreMatrix, glm::vec3(A.x, A.y, A.z));
		return matrix;
	}

	static Matrix Rotate(const Vector3& A)
	{
		Matrix result(1);
		result.coreMatrix = glm::rotate(result.coreMatrix, Math::Radians(A.x), glm::vec3(1, 0, 0));
		result.coreMatrix = glm::rotate(result.coreMatrix, Math::Radians(A.y), glm::vec3(0, 1, 0));
		result.coreMatrix = glm::rotate(result.coreMatrix, Math::Radians(A.z), glm::vec3(0, 0, 1));
		return result;
	}

	static Matrix Rotate(Matrix& matrix, const Vector3& A)
	{
		matrix.coreMatrix = glm::rotate(matrix.coreMatrix, Math::Radians(A.x), glm::vec3(1, 0, 0));
		matrix.coreMatrix = glm::rotate(matrix.coreMatrix, Math::Radians(A.y), glm::vec3(0, 1, 0));
		matrix.coreMatrix = glm::rotate(matrix.coreMatrix, Math::Radians(A.z), glm::vec3(0, 0, 1));
		return matrix;
	}

	static Matrix Perspective(float fovY, float aspect, float nearZ, float farZ)
	{
		Matrix result;
		result.coreMatrix = glm::mat4(1.0f);
		result.coreMatrix = glm::perspective(fovY, aspect, nearZ, farZ);
		return result;
	}

	static Matrix Orthographic(float left, float right, float bottom, float top)
	{
		Matrix result;
		result.coreMatrix = glm::mat4(1.0f);
		result.coreMatrix = glm::ortho(left, right, bottom, top);
		return result;
	}

	static Matrix LookAt(Vector3 eye, Vector3 center, Vector3 up)
	{
		Matrix result;

		/*std::cout << eye.x << " " << eye.y << " " << eye.z << std::endl;
		std::cout << center.x << " " << center.y << " " << center.z << std::endl;
		std::cout << up.x << " " << up.y << " " << up.z << std::endl;*/
		result.coreMatrix = glm::mat4(1.0f);
		result.coreMatrix = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(center.x, center.y, center.z), glm::vec3(up.x, up.y, up.z));
		return result;
	}
};
