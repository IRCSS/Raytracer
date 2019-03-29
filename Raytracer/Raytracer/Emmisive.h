#pragma once
#include "material.h"
class Emmisive : public material
{
public:
	Emmisive();

	Emmisive(const vec3& a) : emmisiveFactor(a) {};
	virtual bool scatter(const Ray&  r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, vec3& emmisve, std::uniform_real_distribution<float> &d, std::mt19937 &g) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere(d, g);
		scattered = Ray(rec.p, target - rec.p);
		attenuation = vec3(1.0f);
		emmisve = emmisiveFactor;
		return true;
	}
	vec3 emmisiveFactor;
	~Emmisive();
};

