#include "Math.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model;
struct Color;
struct Vector3;
struct Vector4;

/*
The `template<typename T>` is only while RobotoSkunk stops being lazy and gets to work on a more flexible and logical definition
*/

struct Vector2 {

	float x, y;

	std::vector<Model*> sharedModels;

	void SendChangeEvent();

	Vector2(): x(0.f), y(0.f) {}

	template <typename T1, typename T2> Vector2(T1 x, T2 y) : x((float)x), y((float)y) { }

	Vector2(const Vector4& A);
	Vector2(const Vector3& A);
	//Vector2(Vector2 A);
	Vector2(const Color& A);

	Vector2(const float& A) : x(A), y(A) {}

	//Doing the operators in the .h will cause the compiler to go bananas.
	//Please put every function that combines other structs/classes in this header into the .cpp to avoid any conflict :D
	Vector2 operator=(const Vector2& A);
	Vector2 operator=(const Vector3& A);
	Vector2 operator=(const Vector4& A);
	Vector2 operator=(const Color& A);
	Vector2 operator=(const float& A);

	Vector2 operator+=(const Vector2& A);
	Vector2 operator+=(const Vector3& A);
	Vector2 operator+=(const Vector4& A);
	Vector2 operator+=(const Color& A);
	Vector2 operator+=(const float& A);

	Vector2 operator+(const Vector2& A) { Vector2 result = Vector2(x, y); return result += A; }
	Vector2 operator+(const Vector3& A) { Vector2 result = Vector2(x, y); return result += A; }
	Vector2 operator+(const Vector4& A) { Vector2 result = Vector2(x, y); return result += A; }
	Vector2 operator+(const Color& A) { Vector2 result = Vector2(x, y); return result += A; }
	Vector2 operator+(const float& A) { Vector2 result = Vector2(x, y); return result += A; }

	Vector2 operator-=(const Vector2& A);
	Vector2 operator-=(const Vector3& A);
	Vector2 operator-=(const Vector4& A);
	Vector2 operator-=(const Color& A);
	Vector2 operator-=(const float& A);

	Vector2 operator-(const Vector2& A) { Vector2 result = Vector2(x, y); return result -= A; }
	Vector2 operator-(const Vector3& A) { Vector2 result = Vector2(x, y); return result -= A; }
	Vector2 operator-(const Vector4& A) { Vector2 result = Vector2(x, y); return result -= A; }
	Vector2 operator-(const Color& A) { Vector2 result = Vector2(x, y); return result -= A; }
	Vector2 operator-(const float& A) { Vector2 result = Vector2(x, y); return result -= A; }

	Vector2 operator*=(const Vector2& A);
	Vector2 operator*=(const Vector3& A);
	Vector2 operator*=(const Vector4& A);
	Vector2 operator*=(const Color& A);
	Vector2 operator*=(const float& A);

	Vector2 operator*(const Vector2& A) { Vector2 result = Vector2(x, y); return result *= A; }
	Vector2 operator*(const Vector3& A) { Vector2 result = Vector2(x, y); return result *= A; }
	Vector2 operator*(const Vector4& A) { Vector2 result = Vector2(x, y); return result *= A; }
	Vector2 operator*(const Color& A) { Vector2 result = Vector2(x, y); return result *= A; }
	Vector2 operator*(const float& A) { Vector2 result = Vector2(x, y); return result *= A; }

	Vector2 operator/=(const Vector2& A);
	Vector2 operator/=(const Vector3& A);
	Vector2 operator/=(const Vector4& A);
	Vector2 operator/=(const Color& A);
	Vector2 operator/=(const float& A);

	Vector2 operator/(const Vector2& A) { Vector2 result = Vector2(x, y); return result /= A; }
	Vector2 operator/(const Vector3& A) { Vector2 result = Vector2(x, y); return result /= A; }
	Vector2 operator/(const Vector4& A) { Vector2 result = Vector2(x, y); return result /= A; }
	Vector2 operator/(const Color& A) { Vector2 result = Vector2(x, y); return result /= A; }
	Vector2 operator/(const float& A) { Vector2 result = Vector2(x, y); return result /= A; }

	friend bool operator==(Vector2& l, Vector2& r);
	friend bool operator==(Vector2& l, Vector3& r);
	friend bool operator==(Vector2& l, Vector4& r);
	friend bool operator==(Vector2& l, Color& r);

	friend bool operator!=(Vector2& l, Vector2& r);
	friend bool operator!=(Vector2& l, Vector3& r);
	friend bool operator!=(Vector2& l, Vector4& r);
	friend bool operator!=(Vector2& l, Color& r);

	friend bool operator>=(Vector2& l, Vector2& r);
	friend bool operator>=(Vector2& l, Vector3& r);
	friend bool operator>=(Vector2& l, Vector4& r);
	friend bool operator>=(Vector2& l, Color& r);

	friend bool operator<=(Vector2& l, Vector2& r);
	friend bool operator<=(Vector2& l, Vector3& r);
	friend bool operator<=(Vector2& l, Vector4& r);
	friend bool operator<=(Vector2& l, Color& r);

	friend bool operator>(Vector2& l, Vector2& r);
	friend bool operator>(Vector2& l, Vector3& r);
	friend bool operator>(Vector2& l, Vector4& r);
	friend bool operator>(Vector2& l, Color& r);

	friend bool operator<(Vector2& l, Vector2& r);
	friend bool operator<(Vector2& l, Vector3& r);
	friend bool operator<(Vector2& l, Vector4& r);
	friend bool operator<(Vector2& l, Color& r);

	static Vector2 one() { return Vector2(1, 1); }
	static Vector2 zero() { return Vector2(0, 0); }
	static Vector2 negative() { return Vector2(-1, -1); }
	static Vector2 left() { return Vector2(-1, 0); }
	static Vector2 right() { return Vector2(1, 0); }
	static Vector2 up() { return Vector2(0, 1); }
	static Vector2 down() { return Vector2(0, -1); }

	static float Distance(Vector2 v1, Vector2 v2) {
		return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
	}

	static Vector2 ClampRangeMagnitude(Vector2 v, float m) {
		return Vector2(ClampRange(v.x, -m, m), ClampRange(v.y, -m, m));
	}
	static Vector2 ClampRangeMagnitude(Vector2 v, float _min, float _max) {
		return Vector2(ClampRange(v.x, _min, _max), ClampRange(v.y, _min, _max));
	}

	static Vector2 Random() {
		return Vector2(rand(), rand());
	}
	static Vector2 Random(float m) {
		return Vector2(rand() * m, rand() * m);
	}
	static Vector2 Random(float _min, float _max) {
		return Vector2(Math::RandomRange(_min, _max), Math::RandomRange(_min, _max));
	}

	static Vector2 Min(Vector2 v1, Vector2 v2) {
		return Vector2(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
	}
	static Vector2 Max(Vector2 v1, Vector2 v2) {
		return Vector2(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
	}

	static Vector2 Normalize(Vector2 v) {
		float m = Distance(Vector2::zero(), v);
		if (m == 0.f) {
			return Vector2::zero();
		}

		return Vector2(
			v.x / m,
			v.y / m
		);
	}
};

struct Vector3 {

	float x, y, z;

	std::vector<Model*> sharedModels;

	void SendChangeEvent();

	Vector3() : x(0), y(0), z(0) {}

	template <typename T1, typename T2> Vector3(T1 x, T2 y) : x((float)x), y((float)y), z(0) {}
	template <typename T1, typename T2, typename T3> Vector3(T1 x, T2 y, T3 z) : x((float)x), y((float)y), z((float)z) {}

	Vector3(const Vector2& A);
	//Vector3(Vector3& A);
	Vector3(const Vector4& A);
	Vector3(const Color& A);

	Vector3(const float& A) : x(A), y(A), z(A) {}

	//Doing the operators in the .h will cause the compiler to go bananas.
	//Please put every function that combines other structs/classes in this header into the .cpp to avoid any conflict :D
	Vector3 operator=(const Vector2& A);
	Vector3 operator=(const Vector3& A);
	Vector3 operator=(const Vector4& A);
	Vector3 operator=(const Color& A);
	Vector3 operator=(const float& A);

	Vector3 operator+=(const Vector2& A);
	Vector3 operator+=(const Vector3& A);
	Vector3 operator+=(const Vector4& A);
	Vector3 operator+=(const Color& A);
	Vector3 operator+=(const float& A);

	Vector3 operator+(const Vector2& A) { Vector3 result = Vector3(x, y, z); return result += A; }
	Vector3 operator+(const Vector3& A) { Vector3 result = Vector3(x, y, z); return result += A; }
	Vector3 operator+(const Vector4& A) { Vector3 result = Vector3(x, y, z); return result += A; }
	Vector3 operator+(const Color& A) { Vector3 result = Vector3(x, y, z); return result += A; }
	Vector3 operator+(const float& A) { Vector3 result = Vector3(x, y, z); return result += A; }

	Vector3 operator-=(const Vector2& A);
	Vector3 operator-=(const Vector3& A);
	Vector3 operator-=(const Vector4& A);
	Vector3 operator-=(const Color& A);
	Vector3 operator-=(const float& A);

	Vector3 operator-(const Vector2& A) { Vector3 result = Vector3(x, y, z); return result -= A; }
	Vector3 operator-(const Vector3& A) { Vector3 result = Vector3(x, y, z); return result -= A; }
	Vector3 operator-(const Vector4& A) { Vector3 result = Vector3(x, y, z); return result -= A; }
	Vector3 operator-(const Color& A) { Vector3 result = Vector3(x, y, z); return result -= A; }
	Vector3 operator-(const float& A) { Vector3 result = Vector3(x, y, z); return result -= A; }

	Vector3 operator*=(const Vector2& A);
	Vector3 operator*=(const Vector3& A);
	Vector3 operator*=(const Vector4& A);
	Vector3 operator*=(const Color& A);
	Vector3 operator*=(const float& A);

	Vector3 operator*(const Vector2& A) { Vector3 result = Vector3(x, y, z); return result *= A; }
	Vector3 operator*(const Vector3& A) { Vector3 result = Vector3(x, y, z); return result *= A; }
	Vector3 operator*(const Vector4& A) { Vector3 result = Vector3(x, y, z); return result *= A; }
	Vector3 operator*(const Color& A) { Vector3 result = Vector3(x, y, z); return result *= A; }
	Vector3 operator*(const float& A) { Vector3 result = Vector3(x, y, z); return result *= A; }

	Vector3 operator/=(const Vector2& A);
	Vector3 operator/=(const Vector3& A);
	Vector3 operator/=(const Vector4& A);
	Vector3 operator/=(const Color& A);
	Vector3 operator/=(const float& A);

	Vector3 operator/(const Vector2& A) { Vector3 result = Vector3(x, y, z); return result /= A; }
	Vector3 operator/(const Vector3& A) { Vector3 result = Vector3(x, y, z); return result /= A; }
	Vector3 operator/(const Vector4& A) { Vector3 result = Vector3(x, y, z); return result /= A; }
	Vector3 operator/(const Color& A) { Vector3 result = Vector3(x, y, z); return result /= A; }
	Vector3 operator/(const float& A) { Vector3 result = Vector3(x, y, z); return result /= A; }

	friend bool operator==(Vector3& l, Vector2& r);
	friend bool operator==(Vector3& l, Vector3& r);
	friend bool operator==(Vector3& l, Vector4& r);
	friend bool operator==(Vector3& l, Color& r);

	friend bool operator!=(Vector3& l, Vector2& r);
	friend bool operator!=(Vector3& l, Vector3& r);
	friend bool operator!=(Vector3& l, Vector4& r);
	friend bool operator!=(Vector3& l, Color& r);

	friend bool operator>=(Vector3& l, Vector2& r);
	friend bool operator>=(Vector3& l, Vector3& r);
	friend bool operator>=(Vector3& l, Vector4& r);
	friend bool operator>=(Vector3& l, Color& r);

	friend bool operator<=(Vector3& l, Vector2& r);
	friend bool operator<=(Vector3& l, Vector3& r);
	friend bool operator<=(Vector3& l, Vector4& r);
	friend bool operator<=(Vector3& l, Color& r);

	friend bool operator>(Vector3& l, Vector2& r);
	friend bool operator>(Vector3& l, Vector3& r);
	friend bool operator>(Vector3& l, Vector4& r);
	friend bool operator>(Vector3& l, Color& r);

	friend bool operator<(Vector3& l, Vector2& r);
	friend bool operator<(Vector3& l, Vector3& r);
	friend bool operator<(Vector3& l, Vector4& r);
	friend bool operator<(Vector3& l, Color& r);

	static Vector3 one() { return Vector3(1, 1); }
	static Vector3 zero() { return Vector3(0, 0); }
	static Vector3 negative() { return Vector3(-1, -1); }
	static Vector3 left() { return Vector3(-1, 0); }
	static Vector3 right() { return Vector3(0, -1); }
	static Vector3 up() { return Vector3(0, 1); }
	static Vector3 down() { return Vector3(0, -1); }
	static Vector3 back() { return Vector3(0, 0, -1); }
	static Vector3 front() { return Vector3(0, 0, 1); }

	static Vector3 Lerp(Vector3 pointA, Vector3 pointB, float time);

	float Magnitude();

	static float Distance(Vector3 v1, Vector3 v2) {
		return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
	}

	static Vector3 ClampRangeMagnitude(Vector3 v, float m) {
		return Vector3(ClampRange(v.x, -m, m), ClampRange(v.y, -m, m), ClampRange(v.z, -m, m));
	}
	static Vector3 ClampRangeMagnitude(Vector3 v, float _min, float _max) {
		return Vector3(ClampRange(v.x, _min, _max), ClampRange(v.y, _min, _max), ClampRange(v.z, _min, _max));
	}

	static Vector3 Random() {
		return Vector3(rand(), rand(), rand());
	}
	static Vector3 Random(float m) {
		return Vector3(rand() * m, rand() * m, rand() * m);
	}
	static Vector3 Random(float _min, float _max) {
		return Vector3(Math::RandomRange(_min, _max), Math::RandomRange(_min, _max), Math::RandomRange(_min, _max));
	}

	static Vector3 Min(Vector3 v1, Vector3 v2) {
		return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	static Vector3 Max(Vector3 v1, Vector3 v2) {
		return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	static Vector3 Normalize(Vector3 v) {
		float m = Distance(Vector3::zero(), v);
		if (m == 0.f) {
			return Vector3::zero();
		}
		
		return Vector3(
			v.x / m,
			v.y / m,
			v.z / m
		);
	}

	static float Dot(Vector3 one, Vector3 two)
	{
		return one.x * two.x + one.y * two.y + one.z * two.z;
	}

	static Vector3 Cross(Vector3 one, Vector3 two)
	{
		glm::vec3 oneM, twoM, result;
		oneM = glm::vec3(one.x, one.y, one.z);
		twoM = glm::vec3(two.x, two.y, two.z);

		result = glm::cross(oneM, twoM);

		return Vector3(result.x, result.y, result.z);

		/*return Vector3(one.y * two.z - two.z * one.y,
			one.z * two.x - two.x * one.z,
			one.x * two.y - two.y * one.x);*/
	}
};

struct Vector4 {
	float x, y, z, w;

	std::vector<Model*> sharedModels;

	bool eventEnabled = true;

	bool& EventEnabled() { return eventEnabled; }

	void SendChangeEvent();

	Vector4() : x(0), y(0), z(0), w(0) {}

	template <typename T1, typename T2> Vector4(T1 x, T2 y) : x((float)x), y((float)y), z(0), w(0) {}
	template <typename T1, typename T2, typename T3> Vector4(T1 x, T2 y, T3 z) : x((float)x), y((float)y), z((float)z), w(0) {}
	template <typename T1, typename T2, typename T3, typename T4> Vector4(T1 x, T2 y, T3 z, T4 w) : x((float)x), y((float)y), z((float)z), w((float)w) {}

	Vector4(const Vector2& A);
	Vector4(const Vector3& A);
	//Vector4(Vector4 A);
	Vector4(const Color& A);

	Vector4(const float& A) : x(A), y(A), z(A), w(A) {}

	//Doing the operators in the .h will cause the compiler to go bananas.
//Please put every function that combines other structs/classes in this header into the .cpp to avoid any conflict :D
	Vector4 operator=(const Vector2& A);
	Vector4 operator=(const Vector3& A);
	Vector4 operator=(const Vector4& A);
	Vector4 operator=(const Color& A);
	Vector4 operator=(const float& A);

	Vector4 operator+=(const Vector2& A);
	Vector4 operator+=(const Vector3& A);
	Vector4 operator+=(const Vector4& A);
	Vector4 operator+=(const Color& A);
	Vector4 operator+=(const float& A);

	Vector4 operator+(const Vector2& A) { Vector4 result = Vector4(x, y, z, w); return result += A; }
	Vector4 operator+(const Vector3& A) { Vector4 result = Vector4(x, y, z, w); return result += A; }
	Vector4 operator+(const Vector4& A) { Vector4 result = Vector4(x, y, z, w); return result += A; }
	Vector4 operator+(const Color& A) { Vector4 result = Vector4(x, y, z, w); return result += A; }
	Vector4 operator+(const float& A) { Vector4 result = Vector4(x, y, z, w); return result += A; }

	Vector4 operator-=(const Vector2& A);
	Vector4 operator-=(const Vector3& A);
	Vector4 operator-=(const Vector4& A);
	Vector4 operator-=(const Color& A);
	Vector4 operator-=(const float& A);

	Vector4 operator-(const Vector2& A) { Vector4 result = Vector4(x, y, z, w); return result -= A; }
	Vector4 operator-(const Vector3& A) { Vector4 result = Vector4(x, y, z, w); return result -= A; }
	Vector4 operator-(const Vector4& A) { Vector4 result = Vector4(x, y, z, w); return result -= A; }
	Vector4 operator-(const Color& A) { Vector4 result = Vector4(x, y, z, w); return result -= A; }
	Vector4 operator-(const float& A) { Vector4 result = Vector4(x, y, z, w); return result -= A; }

	Vector4 operator*=(const Vector2& A);
	Vector4 operator*=(const Vector3& A);
	Vector4 operator*=(const Vector4& A);
	Vector4 operator*=(const Color& A);
	Vector4 operator*=(const float& A);

	Vector4 operator*(const Vector2& A) { Vector4 result = Vector4(x, y, z, w); return result *= A; }
	Vector4 operator*(const Vector3& A) { Vector4 result = Vector4(x, y, z, w); return result *= A; }
	Vector4 operator*(const Vector4& A) { Vector4 result = Vector4(x, y, z, w); return result *= A; }
	Vector4 operator*(const Color& A) { Vector4 result = Vector4(x, y, z, w); return result *= A; }
	Vector4 operator*(const float& A) { Vector4 result = Vector4(x, y, z, w); return result *= A; }

	Vector4 operator/=(const Vector2& A);
	Vector4 operator/=(const Vector3& A);
	Vector4 operator/=(const Vector4& A);
	Vector4 operator/=(const Color& A);
	Vector4 operator/=(const float& A);

	Vector4 operator/(const Vector2& A) { Vector4 result = Vector4(x, y, z, w); return result /= A; }
	Vector4 operator/(const Vector3& A) { Vector4 result = Vector4(x, y, z, w); return result /= A; }
	Vector4 operator/(const Vector4& A) { Vector4 result = Vector4(x, y, z, w); return result /= A; }
	Vector4 operator/(const Color& A) { Vector4 result = Vector4(x, y, z, w); return result /= A; }
	Vector4 operator/(const float& A) { Vector4 result = Vector4(x, y, z, w); return result /= A; }

	float* ValuePointer()
	{
		float points[3];
		points[0] = x;
		points[1] = y;
		points[2] = z;
		return points;
	}

	friend bool operator==(Vector4& l, Vector2& r);
	friend bool operator==(Vector4& l, Vector3& r);
	friend bool operator==(Vector4& l, Vector4& r);
	friend bool operator==(Vector4& l, Color& r);

	friend bool operator!=(Vector4& l, Vector2& r);
	friend bool operator!=(Vector4& l, Vector3& r);
	friend bool operator!=(Vector4& l, Vector4& r);
	friend bool operator!=(Vector4& l, Color& r);

	friend bool operator>=(Vector4& l, Vector2& r);
	friend bool operator>=(Vector4& l, Vector3& r);
	friend bool operator>=(Vector4& l, Vector4& r);
	friend bool operator>=(Vector4& l, Color& r);

	friend bool operator<=(Vector4& l, Vector2& r);
	friend bool operator<=(Vector4& l, Vector3& r);
	friend bool operator<=(Vector4& l, Vector4& r);
	friend bool operator<=(Vector4& l, Color& r);

	friend bool operator>(Vector4& l, Vector2& r);
	friend bool operator>(Vector4& l, Vector3& r);
	friend bool operator>(Vector4& l, Vector4& r);
	friend bool operator>(Vector4& l, Color& r);

	friend bool operator<(Vector4& l, Vector2& r);
	friend bool operator<(Vector4& l, Vector3& r);
	friend bool operator<(Vector4& l, Vector4& r);
	friend bool operator<(Vector4& l, Color& r);
	
	static Vector4 one() { return Vector4(1, 1); }
	static Vector4 zero() { return Vector4(0, 0); }
	static Vector4 negative() { return Vector4(-1, -1); }
	static Vector4 left() { return Vector4(-1, 0); }
	static Vector4 right() { return Vector4(0, -1); }
	static Vector4 up() { return Vector4(0, 1); }
	static Vector4 down() { return Vector4(0, -1); }
	static Vector4 back() { return Vector4(0, 0, -1); }
	static Vector4 front() { return Vector4(0, 0, 1); }
	static Vector4 timeup() { return Vector4(0, 0, 0, 1); }
	static Vector4 timedown() { return Vector4(0, 0, 0, -1); }

	static float Distance(Vector4 v1, Vector4 v2) {
		return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2) + pow(v2.w - v1.w, 2));
	}

	static Vector4 ClampRangeMagnitude(Vector4 v, float m) {
		return Vector4(ClampRange(v.x, -m, m), ClampRange(v.y, -m, m), ClampRange(v.z, -m, m), ClampRange(v.w, -m, m));
	}
	static Vector4 ClampRangeMagnitude(Vector4 v, float _min, float _max) {
		return Vector4(ClampRange(v.x, _min, _max), ClampRange(v.y, _min, _max), ClampRange(v.z, _min, _max), ClampRange(v.w, _min, _max));
	}

	static Vector4 Random() {
		return Vector4(rand(), rand(), rand(), rand());
	}
	static Vector4 Random(float m) {
		return Vector4(rand() * m, rand() * m, rand() * m, rand() * m);
	}
	static Vector4 Random(float _min, float _max) {
		return Vector4(Math::RandomRange(_min, _max), Math::RandomRange(_min, _max), Math::RandomRange(_min, _max), Math::RandomRange(_min, _max));
	}

	static Vector4 Min(Vector4 v1, Vector4 v2) {
		return Vector4(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w));
	}
	static Vector4 Max(Vector4 v1, Vector4 v2) {
		return Vector4(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w));
	}

	static Vector4 Normalize(Vector4 v) {
		float m = Distance(Vector4::zero(), v);
		if (m == 0.f) {
			return Vector4::zero();
		}
		
		return Vector4(
			v.x / m,
			v.y / m,
			v.z / m,
			v.w / m
		);
	}

	static float Dot(Vector4 one, Vector4 two)
	{
		return one.x * two.x + one.y * two.y + one.z * two.z + one.w * two.w;
	}

	static Vector4 Cross(Vector4 one, Vector4 two)
	{
		return Vector4(one.w * two.w - one.x * two.x - one.y * two.y - one.z * two.z,
			one.w * two.x + one.x * two.w + one.y * two.z - one.z * two.y,
			one.w * two.y + one.y * two.w + one.z * two.x - one.x * two.z,
			one.w * two.z + one.z * two.w + one.x * two.y - one.y * two.x);
	}
};
