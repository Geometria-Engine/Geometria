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
	bool changedRotation = false;

	if(!editorModeCamera)
	{
		if(_lastEulerAngles != eulerAngles)
		{
			changedRotation = true;
			_lastEulerAngles = eulerAngles;
		}
	}
	else
	{
		if(_lastEulerAngles != editorEulerAngles)
		{
			changedRotation = true;
			_lastEulerAngles = editorEulerAngles;
		}
	}

	if(changedRotation)
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
	
		if(!editorModeCamera)
		{
			rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(eulerAngles.z), glm::vec3(forward.x, forward.y, forward.z));
		}
		else
		{
			rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(editorEulerAngles.z), glm::vec3(forward.x, forward.y, forward.z));
		}
	
		glm::vec3 upGLM = glm::vec3(up.x, up.y, up.z);
		upGLM = glm::mat3(rollMatrix) * upGLM;
	
		up = Vector3(upGLM.x, upGLM.y, upGLM.z);
	}

	UpdateViewProjection();
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

Vector3 Camera::UnProject(Vector3 v)
{
	return Matrix::UnProject(v, _projectionMatrix, ViewMatrix, Vector4(0, 0, Graphics::GetMainWindow().width, Graphics::GetMainWindow().height));
}

double ConvertFrom_Range1_Input_To_Range2_Output(double _input_range_min, 
        double _input_range_max, double _output_range_min, 
        double _output_range_max, double _input_value_tobe_converted)
{
    double diffOutputRange = abs((_output_range_max - _output_range_min));
    double diffInputRange = abs((_input_range_max - _input_range_min));
    double convFactor = (diffOutputRange / diffInputRange);
    return (_output_range_min + (convFactor * (_input_value_tobe_converted - _input_range_min)));
}

Vector3 ScreenToWorldDirection(Matrix proj, Matrix view, Vector2 screen)
{
	Matrix modifiedView = view;
	modifiedView.coreMatrix[3][0] = 0.0f;
	modifiedView.coreMatrix[3][1] = 0.0f;
	modifiedView.coreMatrix[3][2] = 0.0f;

	// WHAT THE FUCK IS GOING ON WITH THE X?!?!?!?!

	float x = (screen.x / Graphics::GetMainWindow().width) * 2 - 1;
	float widthDivided = (float)Graphics::GetMainWindow().width / ((float)Graphics::GetMainWindow().height * 2);
	x = ConvertFrom_Range1_Input_To_Range2_Output(0, widthDivided, 0, 1.05, x);

	//std::cout << x << std::endl;

	float y = (4.0f * screen.y) / Graphics::GetMainWindow().height - 2.0f;

	//std::cout << "Width: " << Graphics::GetMainWindow().width << " || Height: " << Graphics::GetMainWindow().height << "\n";

	Vector2 screenNdc = Vector2(x, y);
	Vector3 world = Matrix::Inverse(proj * modifiedView) * Vector4(
		screenNdc.x, -screenNdc.y, -1.0f, -1.0f);

	return world;
}

Vector3 Camera::GetDirectionFromScreen(Vector2 point)
{
	return ScreenToWorldDirection(_projectionMatrix, ViewMatrix, point);
}