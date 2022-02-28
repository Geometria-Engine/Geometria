#pragma once

#include "../MainAPI/Graphics.h"
#include "../../../Behaviours/Behaviour.h"

class Camera : public ScriptBehaviour
{
public:
	enum direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

	Matrix ViewMatrix, _projectionMatrix, _uiMatrix;
	bool editorModeCamera = true;
	Vector3 editorPosition, editorRotation, editorEulerAngles, eulerAngles, right, forward, up, worldUp;
	float movementSpeed, sensitivity;

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

	inline Matrix GetViewProjection()
	{
		Matrix view;

		if(!editorModeCamera)
			view = _projectionMatrix * Matrix::LookAt(transform.position, transform.position + forward, up);
		else
			view = _projectionMatrix * Matrix::LookAt(editorPosition, editorPosition + forward, up);

		UpdateCameraSpace();
		return view;
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

	void Mouse();
	void MoveDirection(const int direction);
};
