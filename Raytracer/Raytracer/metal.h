#pragma once
#include "material.h"
#include "Utility.h"
class metal : public material
{
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1.0f) fuzz = f; else fuzz = 1.0f; }
	virtual bool scatter(const Ray&  r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, std::uniform_real_distribution<float> &d, std::mt19937 &g) {
		vec3 target = reflect(unit_vector( r_in.direction()), rec.normal);
		scattered = Ray(rec.p, target + fuzz*GeneralUtility::random_in_unit_sphere(d, g) );
		attenuation = albedo;
 
		return (dot(scattered.direction(), rec.normal) >0);
	}
	vec3 albedo; float fuzz;
};

