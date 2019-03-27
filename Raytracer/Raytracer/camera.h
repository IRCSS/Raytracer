#pragma once
#include "Ray.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <random>



class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, std::uniform_real_distribution<float> &d, std::mt19937 &g) {
		
		_d = &d;
		_g = &g;

		lens_radius = aperture / 2.0f;

		float theta = vfov * M_PI / 180.0f;
		float half_height = tan(theta / 2.0f);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		 lower_left_corner = origin - half_width * u * focus_dist - half_height * v* focus_dist - w*focus_dist;
		 horizontal =2.0f*half_width* u* focus_dist;
		 vertical =  2.0f * half_height * v* focus_dist;


		
	};
	Ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
	}



	 vec3 random_in_unit_disk() {
		vec3 p;
		do { 
			p = 2.0f* vec3( (*_d)((*_g)), (*_d)((*_g)), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
		} while (dot(p, p) >= 1.0f);
		return p;
	}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u, v, w;
	float lens_radius;
	std::uniform_real_distribution<float>* _d;
	std::mt19937* _g;

	~camera() ;
};

