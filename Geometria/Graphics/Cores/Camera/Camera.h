#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "geometria.h"

#undef near
#undef far

class Camera : public ScriptBehaviour
{
public:
	enum direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

	Matrix ViewMatrix, _projectionMatrix, _uiMatrix;
	bool editorModeCamera = true;
	Vector3 editorPosition, editorRotation, editorEulerAngles, eulerAngles, _lastEulerAngles, _lastEditorEulerAngles, right, forward, up, worldUp;
	float movementSpeed, sensitivity;
	glm::mat4 rollMatrix;

	float fov = 90;
	float nearPlane = 0.1f, farPlane = 1000.f;

	void OnStartup()
	{
		ClassType = ScriptBehaviour::Class::Object;
		std::cout << "Camera Set!" << std::endl;
	}

	Camera()
	{
	}

	Camera(Vector3 position, float fov, float aspect, float near, float far);

	void UpdateCameraSpace();

	Matrix ProjectionMatrix(float fov, float aspect, float near, float far)
	{
		_projectionMatrix = Matrix::Perspective(fov, aspect, near, far);
		//_uiMatrix = Matrix::Perspective(fov, aspect, near, far * 1000);
		return _projectionMatrix;
	}

	void UpdateViewProjection()
	{
		if(!editorModeCamera)
			ViewMatrix = _projectionMatrix * Matrix::LookAt(transform.position, transform.position + forward, up);
		else
			ViewMatrix = _projectionMatrix * Matrix::LookAt(editorPosition, editorPosition + forward, up);
	}

	inline Matrix GetViewProjection()
	{
		return ViewMatrix;
	}

	Vector3 GetCurrentPosition()
	{
		if (!editorModeCamera)
			return transform.position;

		return editorPosition;
	}

	inline Matrix GetUIProjection(int widthScreen, int heightScreen)
	{
		/*float width = widthScreen / 20.0f;
		float height = heightScreen / 20.0f;
		Vector3 ui = Vector3(0, 0, (width * height) / 2);*/
		//int width = widthScreen;
		//int height = heightScreen;
		float width = widthScreen / 2;
		float height = heightScreen / 2;
		_uiMatrix = Matrix::Orthographic(-width, width, -height, height);
		return _uiMatrix;
	}

	Vector3 UnProject(Vector3 v);

	Vector3 GetDirectionFromScreen(Vector2 point);
	
	void Mouse();
	void MoveDirection(const int direction);
};

#endif
