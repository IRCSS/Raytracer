#pragma once
#include "material.h"

class lambertian : public material
{
public:



	lambertian(const vec3& a) : albedo(a) {};
	virtual bool scatter(const Ray&  r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, vec3& emmisve, std::uniform_real_distribution<float> &d, std::mt19937 &g)  {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere(d, g);
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	
	vec3 albedo;
};

