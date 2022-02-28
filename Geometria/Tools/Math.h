#include <math.h>
#include <random>
/*
I don't know which version of C++ do you use, so I created this definition at the moment
*/
#define ClampRange(_x, _min, _max) (_x > _max ? _max : (_x < _min ? _min : _x))

//LA COSTUMBRE PERDÓN NACHO TE QUIERO XDDDDDDDDDDDDDDDDDDDDDDDDDD
struct Math
{
	//Returns a random value between two numbers
	template<typename T1, typename T2>
	static float RandomRange(T1 x1, T2 x2) {
		return (float)x1 + (rand() * ((float)x2 - (float)x1));
	}

	/*template<typename T>
	static double Radians(T a)
	{
		return double(a * 0.01745329251994329576923690768489);
	}*/

	template<typename T>
	static float Radians(T a)
	{
		return float(a * 0.01745329251994329576923690768489);
	}

	/*template<typename T>
	static double Degrees(T a)
	{
		return double(a * 57.295779513082320876798154814105);
	}*/

	template<typename T>
	static float Degrees(T a)
	{
		return float(a * 57.295779513082320876798154814105);
	}
};