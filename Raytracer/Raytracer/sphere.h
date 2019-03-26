#pragma once
#include "hitable.h"
class material;
class sphere: public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r, material* mat) : center(cen), radius(r) { mt_ptr = mat; };
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	material* mt_ptr;
	~sphere() {
		delete mt_ptr;
	}
};



