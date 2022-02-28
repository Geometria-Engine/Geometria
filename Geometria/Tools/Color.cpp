#include "../Graphics/Externals/Model.h"

void Color::SendChangeEvent()
{
	for (int i = 0; i < sharedModels.size(); i++)
	{
		sharedModels[i]->OnChange(false, false);
	}
}

//==[ SPECIAL REFERENCE CONSTRUCTOR ]==//
Color::Color(const Vector2& A) : r(A.x), g(A.y), b(0), a(0) { }
Color::Color(const Vector3& A) : r(A.x), g(A.y), b(A.z), a(0) { }
Color::Color(const Vector4& A) : r(A.x), g(A.y), b(A.z), a(A.w) { }

//==[ = OPERATOR ]==//
Color Color::operator=(const Vector2& A)
{
	r = A.x;
	g = A.y;
	b = 0;
	a = 0;
	SendChangeEvent();
	return *this;
}

Color Color::operator=(const Vector3& A)
{
	r = A.x;
	g = A.y;
	b = A.z;
	a = 0;
	SendChangeEvent();
	return *this;
}

Color Color::operator=(const Vector4& A)
{
	r = A.x;
	g = A.y;
	b = A.z;
	a = A.w;
	SendChangeEvent();
	return *this;
}

Color Color::operator=(const Color& A)
{
	r = A.r;
	g = A.g;
	b = A.b;
	a = A.a;
	SendChangeEvent();
	return *this;
}

Color Color::operator=(const float& A) {
	r = g = b = a = A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ + OPERATOR ]==//
Color Color::operator+=(const Vector2& A)
{
	r += A.x;
	g += A.y;
	SendChangeEvent();
	return *this;
}

Color Color::operator+=(const Vector3& A)
{
	r += A.x;
	g += A.y;
	b += A.z;
	SendChangeEvent();
	return *this;
}

Color Color::operator+=(const Vector4& A)
{
	r += A.x;
	g += A.y;
	b += A.z;
	a += A.w;
	SendChangeEvent();
	return *this;
}

Color Color::operator+=(const Color& A)
{
	r += A.r;
	g += A.g;
	b += A.b;
	a += A.a;
	SendChangeEvent();
	return *this;
}

Color Color::operator+=(const float& A) {
	r += A;
	g += A;
	b += A;
	a += A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ - OPERATOR ]==//
Color Color::operator-=(const Vector2& A)
{
	r -= A.x;
	g -= A.y;
	SendChangeEvent();
	return *this;
}

Color Color::operator-=(const Vector3& A)
{
	r -= A.x;
	g -= A.y;
	b -= A.z;
	SendChangeEvent();
	return *this;
}

Color Color::operator-=(const Vector4& A)
{
	r -= A.x;
	g -= A.y;
	b -= A.z;
	a -= A.w;
	SendChangeEvent();
	return *this;
}

Color Color::operator-=(const Color& A)
{
	r -= A.r;
	g -= A.g;
	b -= A.b;
	a -= A.a;
	SendChangeEvent();
	return *this;
}

Color Color::operator-=(const float& A) {
	r -= A;
	g -= A;
	b -= A;
	a -= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ * OPERATOR ]==//
Color Color::operator*=(const Vector2& A)
{
	r *= A.x;
	g *= A.y;
	SendChangeEvent();
	return *this;
}

Color Color::operator*=(const Vector3& A)
{
	r *= A.x;
	g *= A.y;
	b *= A.z;
	SendChangeEvent();
	return *this;
}

Color Color::operator*=(const Vector4& A)
{
	r *= A.x;
	g *= A.y;
	b *= A.z;
	a *= A.w;
	SendChangeEvent();
	return *this;
}

Color Color::operator*=(const Color& A)
{
	r *= A.r;
	g *= A.g;
	b *= A.b;
	a *= A.a;
	SendChangeEvent();
	return *this;
}

Color Color::operator*=(const float& A) {
	r *= A;
	g *= A;
	b *= A;
	a *= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ / OPERATOR ]==//
Color Color::operator/=(const Vector2& A)
{
	r /= A.x;
	g /= A.y;
	SendChangeEvent();
	return *this;
}

Color Color::operator/=(const Vector3& A)
{
	r /= A.x;
	g /= A.y;
	b /= A.z;
	SendChangeEvent();
	return *this;
}

Color Color::operator/=(const Vector4& A)
{
	r /= A.x;
	g /= A.y;
	b /= A.z;
	a /= A.w;
	SendChangeEvent();
	return *this;
}

Color Color::operator/=(const Color& A)
{
	r /= A.r;
	g /= A.g;
	b /= A.b;
	a /= A.a;
	SendChangeEvent();
	return *this;
}

Color Color::operator/=(const float& A) {
	r /= A;
	g /= A;
	b /= A;
	a /= A;
	SendChangeEvent();
	return *this;
}
//==================//

//==[ == OPERATOR ]==//
bool operator==(const Color& l, Vector2& r)
{
	return l.r == r.x && l.g == r.y;
}

bool operator==(const Color& l, Vector3& r)
{
	return l.r == r.x && l.g == r.y && l.b == r.z;
}

bool operator==(const Color& l, Vector4& r)
{
	return l.r == r.x && l.g == r.y && l.b == r.z && l.a == r.w;
}

bool operator==(const Color& l, Color& r)
{
	return l.r == r.r && l.g == r.g && l.b == r.b && l.a == r.a;
}
//===================//

//==[ != OPERATOR ]==//
bool operator!=(const Color& l, Vector2& r)
{
	return l.r != r.x || l.g != r.y;
}

bool operator!=(const Color& l, Vector3& r)
{
	return l.r != r.x || l.g != r.y || l.b != r.z;
}

bool operator!=(const Color& l, Vector4& r)
{
	return l.r != r.x || l.g != r.y || l.b != r.z || l.a != r.w;
}

bool operator!=(const Color& l, Color& r)
{
	return l.r != r.r || l.g != r.g || l.b != r.b || l.a != r.a;
}
//====================//

//==[ >= OPERATOR ]==//
bool operator>=(const Color& l, Vector2& r)
{
	return l.r >= r.x || l.g >= r.y;
}

bool operator>=(const Color& l, Vector3& r)
{
	return l.r >= r.x || l.g >= r.y || l.b >= r.z;
}

bool operator>=(const Color& l, Vector4& r)
{
	return l.r >= r.x || l.g >= r.y || l.b >= r.z || l.a >= r.w;
}

bool operator>=(const Color& l, Color& r)
{
	return l.r >= r.r || l.g >= r.g || l.b >= r.b || l.a >= r.a;
}
//====================//

//==[ <= OPERATOR ]==//
bool operator<=(const Color& l, Vector2& r)
{
	return l.r <= r.x || l.g <= r.y;
}

bool operator<=(const Color& l, Vector3& r)
{
	return l.r <= r.x || l.g <= r.y || l.b <= r.z;
}

bool operator<=(const Color& l, Vector4& r)
{
	return l.r <= r.x || l.g <= r.y || l.b <= r.z || l.a <= r.w;
}

bool operator<=(const Color& l, Color& r)
{
	return l.r <= r.r || l.g <= r.g || l.b <= r.b || l.a <= r.a;
}
//====================//

//==[ > OPERATOR ]==//
bool operator>(const Color& l, Vector2& r)
{
	return l.r > r.x || l.g > r.y;
}

bool operator>(const Color& l, Vector3& r)
{
	return l.r > r.x || l.g > r.y || l.b > r.z;
}

bool operator>(const Color& l, Vector4& r)
{
	return l.r > r.x || l.g > r.y || l.b > r.z || l.a > r.w;
}

bool operator>(const Color& l, Color& r)
{
	return l.r > r.r || l.g > r.g || l.b > r.b || l.a > r.a;
}
//===================//

//==[ < OPERATOR ]==//
bool operator<(const Color& l, Vector2& r)
{
	return l.r < r.x || l.g < r.y;
}

bool operator<(const Color& l, Vector3& r)
{
	return l.r < r.x || l.g < r.y || l.b < r.z;
}

bool operator<(const Color& l, Vector4& r)
{
	return l.r < r.x || l.g < r.y || l.b < r.z || l.a < r.w;
}

bool operator<(const Color& l, Color& r)
{
	return l.r < r.r || l.g < r.g || l.b < r.b || l.a < r.a;
}
//===================//
