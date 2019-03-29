#pragma once
#include "hitable.h"
#include <random>
class material
{
public:

	vec3 random_in_unit_sphere(std::uniform_real_distribution<float> &d, std::mt19937 &g) {
		vec3 p;
		do {
			p = 2.0f * vec3(d(g), d(g), d(g)) - vec3(1.0f, 1.0f, 1.0f);
		} while (p.squared_length() >= 1.0);
		return p;
	}

	
	virtual bool scatter(const Ray&  r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, vec3& emmisve, std::uniform_real_distribution<float> &d, std::mt19937 &g) = 0;
	vec3 reflect(const vec3& v, const vec3& n) {
		return  v -2.0f*dot(v,n) *n;
	}

	virtual bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
		vec3 uv = unit_vector(v);
		float dt = dot(uv, n);
		float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1.0f - dt * dt);
		if (discriminant > 0) {
			refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		else return false;
	}

};

