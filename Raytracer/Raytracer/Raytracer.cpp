// Raytracer.cpp : Defines the entry point for the console application.
//
// Random documentaiton: http://www.cplusplus.com/reference/random/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <random>
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "Utility.h"

using namespace std;







vec3 color(const Ray& r, hitable *world, int depth, std::uniform_real_distribution<float> &d, std::mt19937 &g) {
	hit_record rec;

	if (world->hit(r, 0.001f, 3000000.0f, rec)) {
		Ray scattered;
		vec3 attenuation;
		
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, d, g)) {
			
			return attenuation * color(scattered, world, depth + 1, d, g);
		}
		else return vec3(0.0f, 0.0f, 0.0f);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return GeneralUtility::lerp(vec3(1.0f, 1.0f, 1.0f), vec3(0.5f, 0.7f, 1.0f), t);
	}

}


// -----------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	int nx = 200*10;
	int ny = 100*10;
	int ns = 100;


	std::random_device randomDevice;
	std::mt19937 Generator(randomDevice());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	
	hitable *list[5];

	//float R = cos(M_PI / 4.0f);

	//list[0] = new sphere(vec3(-R, 0.0f, -1.0f),R, new lambertian(vec3(0.0f, 0.0f, 1.0f)));
	//list[1] = new sphere(vec3(R, 0.0f, -1.0f),R, new lambertian(vec3(1.0f,0.0f , 0.0f)));

	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.3f)));
	list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.45f, new dielectric(1.8f));
	list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.41f, new metal(vec3(0.8f, 0.8f, 0.8f), 0.0f));
	list[4] = new sphere(vec3(0.0f, -0.4f, -0.4f), -0.10f, new dielectric(1.2f));


	vec3 lookfrom(3.0f, 1.0f, 5.0f);
	vec3 looktat(0.0f, 0.0f, -1.0f);
	float dist_to_focus = (lookfrom - looktat).length();
	float aperture = 2.0f;

	hitable *world = new hitable_list(list, 5);
	camera c(lookfrom, looktat,  vec3(0.0f, 1.0f, 0.0f),30.0f, float(nx)/float( ny), aperture, dist_to_focus, distribution, Generator) ;


	std::ofstream file("output.ppm");

//-----------------------------------------------------------------------------------------------------------------------------------------------


	file << "P3\n" << nx << "  " << ny << "\n255\n";

	for (int j = ny -1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			//c.origin = vec3(0.0f, 0.0f, 1.0f + (distribution(Generator) - 0.5f) / 1.0f);
			vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++) {
				float u = float(i+ distribution(Generator)) / float(nx);
				float v = float(j+ distribution(Generator)) / float(ny);

				
				Ray r = c.get_ray(u, v);

				col += color(r, world, 0, distribution, Generator);
			}

			col /= float(ns);
	
			// gamma correction
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			
	
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			file << ir << " " << ig << " " << ib << "\n";
		}
	} 


// -----------------------------------------------------------------------------------------------------------------------------------------------

	file.close();
}

