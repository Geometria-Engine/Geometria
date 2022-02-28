struct Vector2;
struct Vector3;
struct Vector4;

struct Color
{
	float r, g, b, a;

	std::vector<Model*> sharedModels;

	void SendChangeEvent();

	Color() : r(1), g(1), b(1), a(1) {}

	/*Color(const Vector2& A) : r(A.x), g(A.y) {}
	Color(const Vector3& A) : r(A.x), g(A.y), b(A.z) {}
	Color(const Vector4& A) : r(A.x), g(A.y), b(A.z), a(A.w) {}*/

	Color(const Vector2& A);
	Color(const Vector3& A);
	Color(const Vector4& A);

	template <typename T1, typename T2, typename T3> Color(T1 r, T2 g, T3 b) : r((float)r), g((float)g), b((float)b), a(1) {}
	template <typename T1, typename T2, typename T3, typename T4> Color(T1 r, T2 g, T3 b, T4 a) : r((float)r), g((float)g), b((float)b), a((float)a) {}

	Color operator=(const Vector2& A);
	Color operator=(const Vector3& A);
	Color operator=(const Vector4& A);
	Color operator=(const Color& A);
	Color operator=(const float& A);

	Color operator+=(const Vector2& A);
	Color operator+=(const Vector3& A);
	Color operator+=(const Vector4& A);
	Color operator+=(const Color& A);
	Color operator+=(const float& A);

	Color operator+(const Vector2& A) { Color result = *this; return result += A; }
	Color operator+(const Vector3& A) { Color result = *this; return result += A; }
	Color operator+(const Vector4& A) { Color result = *this; return result += A; }
	Color operator+(const Color& A) { Color result = *this; return result += A; }
	Color operator+(const float& A) { Color result = *this; return result += A; }

	Color operator-=(const Vector2& A);
	Color operator-=(const Vector3& A);
	Color operator-=(const Vector4& A);
	Color operator-=(const Color& A);
	Color operator-=(const float& A);

	Color operator-(const Vector2& A) { Color result = *this; return result -= A; }
	Color operator-(const Vector3& A) { Color result = *this; return result -= A; }
	Color operator-(const Vector4& A) { Color result = *this; return result -= A; }
	Color operator-(const Color& A) { Color result = *this; return result -= A; }
	Color operator-(const float& A) { Color result = *this; return result -= A; }

	Color operator*=(const Vector2& A);
	Color operator*=(const Vector3& A);
	Color operator*=(const Vector4& A);
	Color operator*=(const Color& A);
	Color operator*=(const float& A);

	Color operator*(const Vector2& A) { Color result = *this; return result *= A; }
	Color operator*(const Vector3& A) { Color result = *this; return result *= A; }
	Color operator*(const Vector4& A) { Color result = *this; return result *= A; }
	Color operator*(const Color& A) { Color result = *this; return result *= A; }
	Color operator*(const float& A) { Color result = *this; return result *= A; }

	Color operator/=(const Vector2& A);
	Color operator/=(const Vector3& A);
	Color operator/=(const Vector4& A);
	Color operator/=(const Color& A);
	Color operator/=(const float& A);

	Color operator/(const Vector2& A) { Color result = *this; return result /= A; }
	Color operator/(const Vector3& A) { Color result = *this; return result /= A; }
	Color operator/(const Vector4& A) { Color result = *this; return result /= A; }
	Color operator/(const Color& A) { Color result = *this; return result /= A; }
	Color operator/(const float& A) { Color result = *this; return result /= A; }

	friend bool operator==(const Color& l, Vector2& r);
	friend bool operator==(const Color& l, Vector3& r);
	friend bool operator==(const Color& l, Vector4& r);
	friend bool operator==(const Color& l, Color& r);

	friend bool operator!=(const Color& l, Vector2& r);
	friend bool operator!=(const Color& l, Vector3& r);
	friend bool operator!=(const Color& l, Vector4& r);
	friend bool operator!=(const Color& l, Color& r);

	friend bool operator>=(const Color& l, Vector2& r);
	friend bool operator>=(const Color& l, Vector3& r);
	friend bool operator>=(const Color& l, Vector4& r);
	friend bool operator>=(const Color& l, Color& r);

	friend bool operator<=(const Color& l, Vector2& r);
	friend bool operator<=(const Color& l, Vector3& r);
	friend bool operator<=(const Color& l, Vector4& r);
	friend bool operator<=(const Color& l, Color& r);

	friend bool operator>(const Color& l, Vector2& r);
	friend bool operator>(const Color& l, Vector3& r);
	friend bool operator>(const Color& l, Vector4& r);
	friend bool operator>(const Color& l, Color& r);

	friend bool operator<(const Color& l, Vector2& r);
	friend bool operator<(const Color& l, Vector3& r);
	friend bool operator<(const Color& l, Vector4& r);
	friend bool operator<(const Color& l, Color& r);

	static Color white() { return Color(1, 1, 1, 1); }
	static Color black() { return Color(0, 0, 0, 1); }
	static Color red() { return Color(1, 0, 0, 1); }
	static Color green() { return Color(0, 1, 0, 1); }
	static Color blue() { return Color(0, 0, 1, 1); }
	static Color yellow() { return Color(1, 1, 0, 1); }
	static Color cyan() { return Color(0, 1, 1, 1); }
	static Color pink() { return Color(1, 0, 1, 1); }
	static Color gray() { return Color(0.5f, 0.5f, 0.5f, 1); }
	static Color darkGray() { return Color(0.15f, 0.15f, 0.15f, 1); }
	static Color transparent() { return Color(0, 0, 0, 0); }

	Color ClampRangeMagnitude(Color c, float m) {
		return Color(ClampRange(c.r, -m, m), ClampRange(c.g, -m, m), ClampRange(c.b, -m, m), ClampRange(c.a, -m, m));
	}
	Color ClampRangeMagnitude(const Color c, float _min, float _max) {
		return Color(ClampRange(c.r, _min, _max), ClampRange(c.g, _min, _max), ClampRange(c.b, _min, _max), ClampRange(c.a, _min, _max));
	}

	Color HSVtoRGB(float h, float s, float v, float a = 1.f){
		h = ClampRange(h, 0.f, 360.f);
		s = ClampRange(s, 0.f, 1.f);
		v = ClampRange(v, 0.f, 1.f);

		float c = v * s;
		float x = c * (1.f - fabs(fmod((h/60.f), 2.f) - 1.f));
		float m = v-c;

		float rgb[3] = {0, 0, 0};

		if(0.f <= h && h < 60.f){rgb[0] = c; rgb[1] = x;}
		else if(60.f <= h && h < 120.f){rgb[0] = x; rgb[1] = c;}
		else if(120.f <= h && h < 180.f){rgb[1] = c; rgb[2] = x;}
		else if(180.f <= h && h < 240.f){rgb[1] = x; rgb[2] = c;}
		else if(240.f <= h && h < 300.f){rgb[0] = x; rgb[2] = c;}
		else if(300.f <= h && h < 360.f){rgb[0] = c; rgb[2] = x;}

		rgb[0] += m; rgb[1] += m; rgb[2] += m;
		rgb[0] *= 255.f; rgb[1] *= 255.f; rgb[2] *= 255.f;

		return Color(rgb[0]/255.f, rgb[1]/255.f, rgb[2]/255.f, a);
	}

	float * RGBtoHSV(Color c){
		float *hsv = new float[3];
		float cmax = std::max(c.r, std::max(c.g, c.b));
		float cmin = std::min(c.r, std::min(c.g, c.b));
		float diff = cmax - cmin;
		
		//Hue
		if (cmax == c.r) hsv[0] = 60.f * fmod(((c.g - c.b)/diff), 6.f);
		else if (cmax == c.g) hsv[0] = 60.f * ((c.b - c.r)/diff + 2.f);
		else if (cmax == c.b) hsv[0] = 60.f * ((c.r - c.g)/diff + 4.f);

		//Saturation
		if (cmax != 0) hsv[1] = diff/cmax;

		//Value
		hsv[2] = cmax; //Bruh...

		return hsv;
	}
};
