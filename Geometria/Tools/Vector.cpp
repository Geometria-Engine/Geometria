#include "../Graphics/Externals/Model.h"

#pragma region Vector2

//==[ SPECIAL REFERENCE CONSTRUCTOR ]==//
//Temporally disabled by RobotoSkunk sempai~
//Vector2::Vector2(Vector2& A) : x(A.x), y(A.y) { }
Vector2::Vector2(const Vector3& A) : x(A.x), y(A.y) { }
Vector2::Vector2(const Vector4& A) : x(A.x), y(A.y) { }
Vector2::Vector2(const Color& A) : x(A.r), y(A.g) { }
//======================================//

//==[ = OPERATOR ]==//
Vector2 Vector2::operator=(const Vector2& A)
{
	x = A.x;
	y = A.y;
	return *this;
}

Vector2 Vector2::operator=(const Vector3& A)
{
	x = A.x;
	y = A.y;
	return *this;
}

Vector2 Vector2::operator=(const Vector4& A)
{
	x = A.x;
	y = A.y;
	return *this;
}

Vector2 Vector2::operator=(const Color& A)
{
	x = A.r;
	y = A.g;
	return *this;
}

Vector2 Vector2::operator=(const float& A) {
	x = y = A;
	return *this;
}
//==================//

//==[ + OPERATOR ]==//
Vector2 Vector2::operator+=(const Vector2& A)
{
	x += A.x;
	y += A.y;
	return *this;
}

Vector2 Vector2::operator+=(const Vector3& A)
{
	x += A.x;
	y += A.y;
	return *this;
}

Vector2 Vector2::operator+=(const Vector4& A)
{
	x += A.x;
	y += A.y;
	return *this;
}

Vector2 Vector2::operator+=(const Color& A)
{
	x += A.r;
	y += A.g;
	return *this;
}

Vector2 Vector2::operator+=(const float& A) {
	x += A;
	y += A;
	return *this;
}
//==================//

//==[ - OPERATOR ]==//
Vector2 Vector2::operator-=(const Vector2& A)
{
	x -= A.x;
	y -= A.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector3& A)
{
	x -= A.x;
	y -= A.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector4& A)
{
	x -= A.x;
	y -= A.y;
	return *this;
}

Vector2 Vector2::operator-=(const Color& A)
{
	x -= A.r;
	y -= A.g;
	return *this;
}

Vector2 Vector2::operator-=(const float& A) {
	x -= A;
	y -= A;
	return *this;
}
//==================//

//==[ * OPERATOR ]==//
Vector2 Vector2::operator*=(const Vector2& A)
{
	x *= A.x;
	y *= A.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector3& A)
{
	x *= A.x;
	y *= A.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector4& A)
{
	x *= A.x;
	y *= A.y;
	return *this;
}

Vector2 Vector2::operator*=(const Color& A)
{
	x *= A.r;
	y *= A.g;
	return *this;
}

Vector2 Vector2::operator*=(const float& A) {
	x *= A;
	y *= A;
	return *this;
}
//==================//

//==[ / OPERATOR ]==//
Vector2 Vector2::operator/=(const Vector2& A)
{
	x /= A.x;
	y /= A.y;
	return *this;
}

Vector2 Vector2::operator/=(const Vector3& A)
{
	x /= A.x;
	y /= A.y;
	return *this;
}

Vector2 Vector2::operator/=(const Vector4& A)
{
	x /= A.x;
	y /= A.y;
	return *this;
}

Vector2 Vector2::operator/=(const Color& A)
{
	x /= A.r;
	y /= A.g;
	return *this;
}

Vector2 Vector2::operator/=(const float& A) {
	x /= A;
	y /= A;
	return *this;
}
//==================//

//==[ == OPERATOR ]==//
bool operator==(Vector2& l, Vector2& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator==(Vector2& l, Vector3& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator==(Vector2& l, Vector4& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator==(Vector2& l, Color& r)
{
	return l.x == r.r && l.y == r.g;
}
//===================//

//==[ != OPERATOR ]==//
bool operator!=(Vector2& l, Vector2& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator!=(Vector2& l, Vector3& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator!=(Vector2& l, Vector4& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator!=(Vector2& l, Color& r)
{
	return l.x != r.r || l.y != r.g;
}
//====================//

//==[ >= OPERATOR ]==//
bool operator>=(Vector2& l, Vector2& r)
{
	return l.x >= r.x || l.y >= r.y;
}

bool operator>=(Vector2& l, Vector3& r)
{
	return l.x >= r.x || l.y >= r.y;
}

bool operator>=(Vector2& l, Vector4& r)
{
	return l.x >= r.x || l.y >= r.y;
}

bool operator>=(Vector2& l, Color& r)
{
	return l.x >= r.r || l.y >= r.g;
}
//====================//

//==[ <= OPERATOR ]==//
bool operator<=(Vector2& l, Vector2& r)
{
	return l.x <= r.x || l.y <= r.y;
}

bool operator<=(Vector2& l, Vector3& r)
{
	return l.x <= r.x || l.y <= r.y;
}

bool operator<=(Vector2& l, Vector4& r)
{
	return l.x <= r.x || l.y <= r.y;
}

bool operator<=(Vector2& l, Color& r)
{
	return l.x <= r.r || l.y <= r.g;
}
//====================//

//==[ > OPERATOR ]==//
bool operator>(Vector2& l, Vector2& r)
{
	return l.x > r.x || l.y > r.y;
}

bool operator>(Vector2& l, Vector3& r)
{
	return l.x > r.x || l.y > r.y;
}

bool operator>(Vector2& l, Vector4& r)
{
	return l.x > r.x || l.y > r.y;
}

bool operator>(Vector2& l, Color& r)
{
	return l.x > r.r || l.y > r.g;
}
//===================//

//==[ < OPERATOR ]==//
bool operator<(Vector2& l, Vector2& r)
{
	return l.x < r.x || l.y < r.y;
}

bool operator<(Vector2& l, Vector3& r)
{
	return l.x < r.x || l.y < r.y;
}

bool operator<(Vector2& l, Vector4& r)
{
	return l.x < r.x || l.y < r.y;
}

bool operator<(Vector2& l, Color& r)
{
	return l.x < r.r || l.y < r.g;
}
//===================//
#pragma endregion

#pragma region Vector3

//==[ SPECIAL REFERENCE CONSTRUCTOR ]==//
Vector3::Vector3(const Vector2& A) : x(A.x), y(A.y), z(0) { }
//Temporally disabled by RobotoSkunk sempai~
//Vector3::Vector3(Vector3& A) : x(A.x), y(A.y), z(A.z) { }
Vector3::Vector3(const Vector4& A) : x(A.x), y(A.y), z(A.z) {}
Vector3::Vector3(const Color& A) : x(A.r), y(A.g), z(A.b) {}
//======================================//

//==[ = OPERATOR ]==//
Vector3 Vector3::operator=(const Vector2& A)
{
	x = A.x;
	y = A.y;
	z = 0;
	return *this;
}

Vector3 Vector3::operator=(const Vector3& A)
{
	x = A.x;
	y = A.y;
	z = A.z;
	return *this;
}

Vector3 Vector3::operator=(const Vector4& A)
{
	x = A.x;
	y = A.y;
	z = A.z;
	return *this;
}

Vector3 Vector3::operator=(const Color& A)
{
	x = A.r;
	y = A.g;
	z = A.b;
	return *this;
}

Vector3 Vector3::operator=(const float& A) {
	x = y = z = A;
	return *this;
}
//==================//

//==[ + OPERATOR ]==//
Vector3 Vector3::operator+=(const Vector2& A)
{
	x += A.x;
	y += A.y;
	return *this;
}

Vector3 Vector3::operator+=(const Vector3& A)
{
	x += A.x;
	y += A.y;
	z += A.z;
	return *this;
}

Vector3 Vector3::operator+=(const Vector4& A)
{
	x += A.x;
	y += A.y;
	z += A.z;
	return *this;
}

Vector3 Vector3::operator+=(const Color& A)
{
	x += A.r;
	y += A.g;
	z += A.b;
	return *this;
}

Vector3 Vector3::operator+=(const float& A) {
	x += A;
	y += A;
	z += A;
	return *this;
}
//==================//

//==[ - OPERATOR ]==//
Vector3 Vector3::operator-=(const Vector2& A)
{
	x -= A.x;
	y -= A.y;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& A)
{
	x -= A.x;
	y -= A.y;
	z -= A.z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector4& A)
{
	x -= A.x;
	y -= A.y;
	z -= A.z;
	return *this;
}

Vector3 Vector3::operator-=(const Color& A)
{
	x -= A.r;
	y -= A.g;
	z -= A.b;
	return *this;
}

Vector3 Vector3::operator-=(const float& A) {
	x -= A;
	y -= A;
	z -= A;
	return *this;
}
//==================//

//==[ * OPERATOR ]==//
Vector3 Vector3::operator*=(const Vector2& A)
{
	x *= A.x;
	y *= A.y;
	return *this;
}

Vector3 Vector3::operator*=(const Vector3& A)
{
	x *= A.x;
	y *= A.y;
	z *= A.z;
	return *this;
}

Vector3 Vector3::operator*=(const Vector4& A)
{
	x *= A.x;
	y *= A.y;
	z *= A.z;
	return *this;
}

Vector3 Vector3::operator*=(const Color& A)
{
	x *= A.r;
	y *= A.g;
	z *= A.b;
	return *this;
}

Vector3 Vector3::operator*=(const float& A) {
	x *= A;
	y *= A;
	z *= A;
	return *this;
}
//==================//

//==[ / OPERATOR ]==//
Vector3 Vector3::operator/=(const Vector2& A)
{
	x /= A.x;
	y /= A.y;
	return *this;
}

Vector3 Vector3::operator/=(const Vector3& A)
{
	x /= A.x;
	y /= A.y;
	z /= A.z;
	return *this;
}

Vector3 Vector3::operator/=(const Vector4& A)
{
	x /= A.x;
	y /= A.y;
	z /= A.z;
	return *this;
}

Vector3 Vector3::operator/=(const Color& A)
{
	x /= A.r;
	y /= A.g;
	z /= A.b;
	return *this;
}

Vector3 Vector3::operator/=(const float& A) {
	x /= A;
	y /= A;
	z /= A;
	return *this;
}
Vector3 Vector3::Lerp(Vector3 pointA, Vector3 pointB, float time)
{
	Vector3 l;
	l.x = pointA.x + time * (pointB.x - pointA.x);
	l.y = pointA.y + time * (pointB.y - pointA.y);
	l.z = pointA.z + time * (pointB.z - pointA.z);
	return l;
}
float Vector3::Magnitude()
{
	float calc = sqrt(x * x + y * y + z * z);
	return calc;
}
//==================//

//==[ == OPERATOR ]==//
bool operator==(Vector3& l, Vector2& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator==(Vector3& l, Vector3& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

bool operator==(Vector3& l, Vector4& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

bool operator==(Vector3& l, Color& r)
{
	return l.x == r.r && l.y == r.g && l.z == r.b;
}
//===================//

//==[ != OPERATOR ]==//
bool operator!=(Vector3& l, Vector2& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator!=(Vector3& l, Vector3& r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z;
}

bool operator!=(Vector3& l, Vector4& r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z;
}

bool operator!=(Vector3& l, Color& r)
{
	return l.x == r.r && l.y == r.g && l.z == r.b;
}
//====================//

//==[ >= OPERATOR ]==//
bool operator>=(Vector3& l, Vector2& r)
{
	return l.x >= r.x || l.y >= r.y;
}

bool operator>=(Vector3& l, Vector3& r)
{
	return l.x >= r.x || l.y >= r.y || l.z >= r.z;
}

bool operator>=(Vector3& l, Vector4& r)
{
	return l.x >= r.x || l.y >= r.y || l.z >= r.z;
}

bool operator>=(Vector3& l, Color& r)
{
	return l.x >= r.r || l.y >= r.g || l.z >= r.b;
}
//====================//

//==[ <= OPERATOR ]==//
bool operator<=(Vector3& l, Vector2& r)
{
	return l.x <= r.x || l.y <= r.y;
}

bool operator<=(Vector3& l, Vector3& r)
{
	return l.x <= r.x || l.y <= r.y || l.z <= r.z;
}

bool operator<=(Vector3& l, Vector4& r)
{
	return l.x <= r.x || l.y <= r.y || l.z <= r.z;
}

bool operator<=(Vector3& l, Color& r)
{
	return l.x <= r.r || l.y <= r.g || l.z <= r.b;
}
//====================//

//==[ > OPERATOR ]==//
bool operator>(Vector3& l, Vector2& r)
{
	return l.x > r.x || l.y > r.y;
}

bool operator>(Vector3& l, Vector3& r)
{
	return l.x > r.x || l.y > r.y || l.z > r.z;
}

bool operator>(Vector3& l, Vector4& r)
{
	return l.x > r.x || l.y > r.y || l.z > r.z;
}

bool operator>(Vector3& l, Color& r)
{
	return l.x > r.r || l.y > r.g || l.z > r.b;
}
//===================//

//==[ < OPERATOR ]==//
bool operator<(Vector3& l, Vector2& r)
{
	return l.x < r.x || l.y < r.y;
}

bool operator<(Vector3& l, Vector3& r)
{
	return l.x < r.x || l.y < r.y || l.z < r.z;
}

bool operator<(Vector3& l, Vector4& r)
{
	return l.x < r.x || l.y < r.y || l.z < r.z;
}

bool operator<(Vector3& l, Color& r)
{
	return l.x < r.r || l.y < r.g || l.z < r.b;
}
//===================//
#pragma endregion

#pragma region Vector4

//==[ SPECIAL REFERENCE CONSTRUCTOR ]==//
Vector4::Vector4(const Vector2& A) : x(A.x), y(A.y), z(0), w(0) { }
Vector4::Vector4(const Vector3& A) : x(A.x), y(A.y), z(A.z), w(0) { }
//Temporally disabled by RobotoSkunk sempai~
//Vector4::Vector4(Vector4& A) : x(A.x), y(A.y), z(A.z), w(A.w) { }
Vector4::Vector4(const Color& A) : x(A.r), y(A.g), z(A.b), w(A.a) { }
//======================================//

void Vector4::SendChangeEvent()
{
	for (int i = 0; i < sharedModels.size(); i++)
	{
		if (EventEnabled())
		{
			sharedModels[i]->OnChange();
		}
	}
}

//==[ = OPERATOR ]==//
Vector4 Vector4::operator=(const Vector2& A)
{
	x = A.x;
	y = A.y;
	z = 0;
	w = 0;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator=(const Vector3& A)
{
	x = A.x;
	y = A.y;
	z = A.z;
	w = 0;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator=(const Vector4& A)
{
	x = A.x;
	y = A.y;
	z = A.z;
	w = A.w;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator=(const Color& A)
{
	x = A.r;
	y = A.g;
	z = A.b;
	w = A.a;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator=(const float& A) {
	x = y = z = w = A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ + OPERATOR ]==//
Vector4 Vector4::operator+=(const Vector2& A)
{
	x += A.x;
	y += A.y;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator+=(const Vector3& A)
{
	x += A.x;
	y += A.y;
	z += A.z;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator+=(const Vector4& A)
{
	x += A.x;
	y += A.y;
	z += A.z;
	w += A.w;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator+=(const Color& A)
{
	x += A.r;
	y += A.g;
	z += A.b;
	w += A.a;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator+=(const float& A) {
	x += A;
	y += A;
	z += A;
	w += A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ - OPERATOR ]==//
Vector4 Vector4::operator-=(const Vector2& A)
{
	x -= A.x;
	y -= A.y;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator-=(const Vector3& A)
{
	x -= A.x;
	y -= A.y;
	z -= A.z;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator-=(const Vector4& A)
{
	x -= A.x;
	y -= A.y;
	z -= A.z;
	w -= A.w;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator-=(const Color& A)
{
	x -= A.r;
	y -= A.g;
	z -= A.b;
	w -= A.a;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator-=(const float& A) {
	x -= A;
	y -= A;
	z -= A;
	w -= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ * OPERATOR ]==//
Vector4 Vector4::operator*=(const Vector2& A)
{
	x *= A.x;
	y *= A.y;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator*=(const Vector3& A)
{
	x *= A.x;
	y *= A.y;
	z *= A.z;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator*=(const Vector4& A)
{
	x *= A.x;
	y *= A.y;
	z *= A.z;
	w *= A.w;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator*=(const Color& A)
{
	x *= A.r;
	y *= A.g;
	z *= A.b;
	w *= A.a;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator*=(const float& A) {
	x *= A;
	y *= A;
	z *= A;
	w *= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ / OPERATOR ]==//
Vector4 Vector4::operator/=(const Vector2& A)
{
	x /= A.x;
	y /= A.y;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator/=(const Vector3& A)
{
	x /= A.x;
	y /= A.y;
	z /= A.z;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator/=(const Vector4& A)
{
	x /= A.x;
	y /= A.y;
	z /= A.z;
	w /= A.w;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator/=(const Color& A)
{
	x /= A.r;
	y /= A.g;
	z /= A.b;
	w /= A.a;
	SendChangeEvent();
	return *this;
}

Vector4 Vector4::operator/=(const float& A) {
	x /= A;
	y /= A;
	z /= A;
	w /= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ == OPERATOR ]==//
bool operator==(Vector4& l, Vector2& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator==(Vector4& l, Vector3& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

bool operator==(Vector4& l, Vector4& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w;
}

bool operator==(Vector4& l, Color& r)
{
	return l.x == r.r && l.y == r.g && l.z == r.b && l.w == r.a;
}
//===================//

//==[ != OPERATOR ]==//
bool operator!=(Vector4& l, Vector2& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator!=(Vector4& l, Vector3& r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z;
}

bool operator!=(Vector4& l, Vector4& r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z || l.w != r.w;
}

bool operator!=(Vector4& l, Color& r)
{
	return l.x != r.r || l.y != r.g || l.z != r.b || l.w != r.a;
}
//====================//

//==[ >= OPERATOR ]==//
bool operator>=(Vector4& l, Vector2& r)
{
	return l.x >= r.x || l.y >= r.y;
}

bool operator>=(Vector4& l, Vector3& r)
{
	return l.x >= r.x || l.y >= r.y || l.z >= r.z;
}

bool operator>=(Vector4& l, Vector4& r)
{
	return l.x >= r.x || l.y >= r.y || l.z >= r.z || l.w >= r.w;
}

bool operator>=(Vector4& l, Color& r)
{
	return l.x >= r.r || l.y >= r.g || l.z >= r.b || l.w >= r.a;
}
//====================//

//==[ <= OPERATOR ]==//
bool operator<=(Vector4& l, Vector2& r)
{
	return l.x <= r.x || l.y <= r.y;
}

bool operator<=(Vector4& l, Vector3& r)
{
	return l.x <= r.x || l.y <= r.y || l.z <= r.z;
}

bool operator<=(Vector4& l, Vector4& r)
{
	return l.x <= r.x || l.y <= r.y || l.z <= r.z || l.w <= r.w;
}

bool operator<=(Vector4& l, Color& r)
{
	return l.x <= r.r || l.y <= r.g || l.z <= r.b || l.w <= r.a;
}
//====================//

//==[ > OPERATOR ]==//
bool operator>(Vector4& l, Vector2& r)
{
	return l.x > r.x || l.y > r.y;
}

bool operator>(Vector4& l, Vector3& r)
{
	return l.x > r.x || l.y > r.y || l.z > r.z;
}

bool operator>(Vector4& l, Vector4& r)
{
	return l.x > r.x || l.y > r.y || l.z > r.z || l.w > r.w;
}

bool operator>(Vector4& l, Color& r)
{
	return l.x > r.r || l.y > r.g || l.z > r.b || l.w > r.a;
}
//===================//

//==[ < OPERATOR ]==//
bool operator<(Vector4& l, Vector2& r)
{
	return l.x < r.x || l.y < r.y;
}

bool operator<(Vector4& l, Vector3& r)
{
	return l.x < r.x || l.y < r.y || l.z < r.z;
}

bool operator<(Vector4& l, Vector4& r)
{
	return l.x < r.x || l.y < r.y || l.z < r.z || l.w < r.w;
}

bool operator<(Vector4& l, Color& r)
{
	return l.x < r.r || l.y < r.g || l.z < r.b || l.w < r.a;
}
//===================//
#pragma endregion
