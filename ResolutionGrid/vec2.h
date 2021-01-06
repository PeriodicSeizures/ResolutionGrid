// common types
#ifndef __VEC2_H__
#define __VEC2_H__

#include <cmath>

#include "lib/fix.h"

//#include "lib/util.h"//

class vec2 {
public:
	FFix x, y;

	vec2() :x(0), y(0) {}
	vec2(FFix x, FFix y) : x(x), y(y) {}
	vec2(const vec2& v) : x(v.x), y(v.y) {}

	bool operator==(const vec2& v) const {
	    return v.x == x && v.y == y;
	}

	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	vec2 operator+(vec2 v) const {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator-(vec2 v) const {
		return vec2(x - v.x, y - v.y);
	}
	vec2 operator*(vec2 v) const {
		return vec2(x * v.x, y * v.y);
	}
	vec2 operator/(vec2 v) const {
		return vec2(x / v.x, y / v.y);
	}

	vec2& operator+=(vec2 v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2& operator-=(vec2 v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	vec2& operator*=(vec2 v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}
	vec2& operator/=(vec2 v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	//vec2 operator+(float s) {
	//	return vec2(x + s, y + s);
	//}
	//vec2 operator-(float s) {
	//	return vec2(x - s, y - s);
	//}

	//vec2 operator*(float s) {
	//	return vec2(x * s, y * s);
	//}
	//vec2 operator/(float s) {
	//	return vec2(x / s, y / s);
	//}
	//
	//vec2 operator*(int s) {
	//	return vec2(x * s, y * s);
	//}
	//vec2 operator/(int s) {
	//	return vec2(x / s, y / s);
	//}

	vec2 operator*(FFix s) const {
		return vec2(x * s, y * s);
	}
	vec2 operator/(FFix s) const {
		return vec2(x / s, y / s);
	}

	vec2& operator*=(FFix s) {
		x *= s;
		y *= s;
		return *this;
	}
	vec2& operator/=(FFix s) {
		x /= s;
		y /= s;
		return *this;
	}

	//vec2& operator+=(float s) {
	//	x += s;
	//	y += s;
	//	return *this;
	//}
	//vec2& operator-=(float s) {
	//	x -= s;
	//	y -= s;
	//	return *this;
	//}
	//vec2& operator*=(float s) {
	//	x *= s;
	//	y *= s;
	//	return *this;
	//}
	//vec2& operator/=(float s) {
	//	x /= s;
	//	y /= s;
	//	return *this;
	//}
	//
	//vec2& operator*=(int s) {
	//	x *= s;
	//	y *= s;
	//	return *this;
	//}
	//vec2& operator/=(int s) {
	//	x /= s;
	//	y /= s;
	//	return *this;
	//}



	//void rotate(float deg) {
	//	float theta = deg / 180.0 * M_PI;
	//	float c = cos(theta);
	//	float s = sin(theta);
	//	float tx = x * c - y * s;
	//	float ty = x * s + y * c;
	//	x = tx;
	//	y = ty;
	//}
	//
	void normalize();
	vec2 normalized();

	//double inline __declspec (naked) __fastcall sqrt14(double n)
	//{
	//	_asm fld qword ptr[esp + 4]
	//		_asm fsqrt
	//	_asm ret 8
	//}

	static float Q_rsqrt(float number)
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long *)&y;                       // evil floating point bit level hacking
		i = 0x5f3759df - (i >> 1);               // what the fuck? 
		y = *(float *)&i;
		y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

		return y;
	}

	//float dist(vec2 v) const {
	//	vec2 d(v.x - x, v.y - y);
	//	return d.length();
	//}
	//
	//float length() const {
	//	//return FFix(1) / Q_rsqrt(x * x + y * y);
	//	return std::sqrt(x * x + y * y);
	//}

	FFix sqDist(vec2 v) const {
		vec2 d(v.x - x, v.y - y);
		return d.sqLength();
	}

	FFix sqLength() const {
		return (x * x) + (y * y);
	}

	vec2 ortho() const {
		return vec2(y, -x);
	}

	static FFix dot(vec2 v1, vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static FFix cross(vec2 v1, vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

};

#endif