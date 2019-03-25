#pragma once
#include "vec3.h"
#include <random>
namespace GeneralUtility {

	inline float frac(float v) {
		return v - floor(v);
	}

	inline vec3 frac3(const vec3& v) {
		return vec3(frac(v.e[0]), frac(v.e[1]), frac(v.e[2]));
	}

	inline vec3 floor3(const vec3& v) {
		return vec3(floor(v.e[0]), floor(v.e[1]), floor(v.e[2]));
	}

	inline vec3 sin3(const vec3& v) {
		return vec3(sin(v.e[0]), sin(v.e[1]), sin(v.e[2]));
	}
	inline float rand418(const int in) {
		return  frac(sin((float)in * 418.0f) * 21.0f);
	}

	 inline vec3 lerp(const vec3 &v1, const vec3 &v2, const float alpha) {
		 return v1 + (v2 - v1)*alpha;
	 }
	 inline float mix(float f1, float f2, float alpha) {
		 return f1 + (f2 - f1)* alpha;
	 }

	 inline float saturate(float f) {
		 return std::fmaxf(0.0f, std::fminf(f, 1.0f));
	 }

	 inline float smoothstep(float a, float b, float x) {
		 float t = saturate((x - b) / (b - a));
		 return t * t*(3.0f - (2.0f*t));
	 }

	 inline vec3 smoothstep3(const vec3& v1, const vec3& v2, float alpha) {
		 return vec3(smoothstep(v1.e[0], v2.e[0], alpha), smoothstep(v1.e[1], v2.e[1], alpha), smoothstep(v1.e[2], v2.e[2], alpha));
	 }


	 inline vec3 smoothstep3(const vec3& v1, const vec3& v2, const vec3& alpha) {
		 return vec3(smoothstep(v1.e[0], v2.e[0], alpha.e[0]), smoothstep(v1.e[1], v2.e[1], alpha.e[1]), smoothstep(v1.e[2], v2.e[2], alpha.e[2]));
	 }


	 inline vec3 random_in_unit_sphere(std::uniform_real_distribution<float> &d, std::mt19937 &g) {
		 vec3 p;
		 do {
			 p = 2.0f * vec3(d(g), d(g), d(g)) - vec3(1.0f, 1.0f, 1.0f);
		 } while (p.squared_length() >= 1.0f);
		 return p;
	 }


	 inline vec3 random3(vec3 st)
	 {
		 st = vec3(dot(st, vec3(127.1f, 311.7f, 211.2f) / 20.0f),
			 dot(st, vec3(269.5f, 183.3f, 157.1f)), dot(st, vec3(269.5f, 183.3f, 17.1f)));
		 return vec3(-1.0f) + 2.0f*frac3(sin3(st)*438.54f);
	 }

	 inline vec3 abs3(vec3 v) {
		 return vec3(abs(v.e[0]), abs(v.e[1]), abs(v.e[2]));
	 }

	 inline float noise3D(vec3 st)
	 {
		 vec3 i = floor3(st);
		 vec3 f = frac3(st);

		 vec3 u = f;

		 float valueNowxy01 = mix(mix(dot(random3(i + vec3(0.0f)), f - vec3(0.0f)),
			 dot(random3(i + vec3(1.0f, 0.0f, 0.0f)), f - vec3(1.0f, 0.0f, 0.0f)), u.x()),
			 mix(dot(random3(i + vec3(0.0f, 1.0f, 0.0f)), f - vec3(0.0f, 1.0f, 0.0f)),
				 dot(random3(i + vec3(1.0f, 1.0f, 0.0f)), f - vec3(1.0f, 1.0f, 0.0f)), u.x()), u.y());
		 float valueNowxy02 = mix(mix(dot(random3(i + vec3(0.0f, 0.0f, 1.0f)), f - vec3(0.0f, 0.0f, 1.0f)),
			 dot(random3(i + vec3(1.0f, 0.0f, 1.0f)), f - vec3(1.0f, 0.0f, 1.0f)), u.x()),
			 mix(dot(random3(i + vec3(0.0f, 1.0f, 1.0f)), f - vec3(0.0f, 1.0f, 1.0f)),
				 dot(random3(i + vec3(1.0f)), f - vec3(1.0f)), u.x()), u.y());

		 return abs(mix(valueNowxy01, valueNowxy02, u.z()));

	 }
}