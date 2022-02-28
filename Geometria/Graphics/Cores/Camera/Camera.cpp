#include "Camera.h"

float Graphics::_deltaTime;
Camera* Graphics::_mainCamera;

Camera::Camera(Vector3 position, float fov, float aspect, float near, float far) :
	forward(Vector3::front()), up(Vector3::up()), worldUp(Vector3::up()),

	fov(fov), nearPlane(near), farPlane(far)
{
	transform.position = position;
	editorPosition = Vector3(0, 0, 2);

	ViewMatrix.coreMatrix = glm::mat4(1.f);

	movementSpeed = 3.f;
	sensitivity = 5.f;


	//TODO: gotta convert these ones in the rotation vector

	//yaw
	eulerAngles.y = 0.f;
	//pitch
	eulerAngles.x = -90.f;
	//roll
	eulerAngles.z = 0.f;

	//yaw
	editorEulerAngles.y = 0.f;
	//pitch
	editorEulerAngles.x = -90.f;
	//roll
	editorEulerAngles.z = 0.f;

	UpdateCameraSpace();
	ProjectionMatrix(fov, aspect, near, far);
}

void Camera::UpdateCameraSpace()
{
	if (!editorModeCamera)
	{
		forward.x = cos(Math::Radians(eulerAngles.x)) * cos(Math::Radians(eulerAngles.y));
		forward.y = sin(Math::Radians(eulerAngles.y));
		forward.z = sin(Math::Radians(eulerAngles.x)) * cos(Math::Radians(eulerAngles.y));
	}
	else
	{
		forward.x = cos(Math::Radians(editorEulerAngles.x)) * cos(Math::Radians(editorEulerAngles.y));
		forward.y = sin(Math::Radians(editorEulerAngles.y));
		forward.z = sin(Math::Radians(editorEulerAngles.x)) * cos(Math::Radians(editorEulerAngles.y));
	}

	forward = Vector3::Normalize(forward);
	right = Vector3::Normalize(Vector3::Cross(forward, worldUp));
	up = Vector3::Normalize(Vector3::Cross(right, forward));
}

void Camera::Mouse()
{
	editorEulerAngles.y -= Graphics::GetMainWindow().Mouse.speedY * sensitivity * Graphics::DeltaTime();
	editorEulerAngles.x += Graphics::GetMainWindow().Mouse.speedX * sensitivity * Graphics::DeltaTime();

	if (editorEulerAngles.y > 80.f)
		editorEulerAngles.y = 80.f;
	else if (editorEulerAngles.y < -80.f)
		editorEulerAngles.y = -80.f;

	if (editorEulerAngles.x > 360.f || editorEulerAngles.x < -360.f)
		editorEulerAngles.x = 0.f;
}

void Camera::MoveDirection(const int direction)
{
	/*switch (direction)
	{
	case FORWARD:
		position += front * movementSpeed * Graphics::DeltaTime();
		break;
	case BACKWARD:
		position -= front * movementSpeed * Graphics::DeltaTime();
		break;
	case LEFT:
		position -= right * movementSpeed * Graphics::DeltaTime();
		break;
	case RIGHT:
		position += right * movementSpeed * Graphics::DeltaTime();
		break;
	case UP:
		position += up * movementSpeed * Graphics::DeltaTime();
		break;
	case DOWN:
		position -= up * movementSpeed * Graphics::DeltaTime();
		break;
	default:
		break;
	}*/
}