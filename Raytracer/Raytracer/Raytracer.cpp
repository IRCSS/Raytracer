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
#include <chrono>
#include <ctime>    
#include <thread>
#include "Emmisive.h"

using namespace std;







vec3 color(const Ray& r, hitable *world, int depth, std::uniform_real_distribution<float> &d, std::mt19937 &g) {
	hit_record rec;

	if (world->hit(r, 0.001f, 3000000.0f, rec)) {
		Ray scattered;
		vec3 attenuation;
		vec3 emmisve = vec3(0.0);
		
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, emmisve, d, g)) {
			
			return emmisve + attenuation * color(scattered, world, depth + 1, d, g);
		}
		else return emmisve;
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return GeneralUtility::lerp(vec3(1.0f), vec3(0.5f, 0.7f, 1.0f), t);
	}

}

hitable *random_scene(std::uniform_real_distribution<float> &d, std::mt19937 &g) {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = d(g);
			vec3 center(a + 0.9f*d(g), 0.2f, b + 0.9f*d(g));
			if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				if (choose_mat < 0.8) list[i++] = new sphere(center, 0.2f, new lambertian(vec3(d(g)* d(g), d(g)* d(g), d(g)* d(g))));
				else if (choose_mat < 0.95f) list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f*(1.0f + d(g)), 0.5f*(1.0f + d(g)), 0.5f*(1.0f + d(g))), 0.5f * d(g) ));
				else list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
			}
		}
	}

	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.9f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(1.0f, 0.04f, 0.04f)));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.4f, 0.5f, 0.5f),  0.1f));
	return new hitable_list(list, i);

}


// -----------------------------------------------------------------------------------------------------------------------------------------------
// MULTI THREADING


struct threadInfo{
	int nx, ny, i, j;
	camera c;
	hitable* world;
	threadInfo(int x, int y, int _i, int _j, camera _c, hitable* _world) : nx(x), ny(y), i(_i), j(_j), c(_c), world(_world){};
};

void accumlate_color(vec3** colorList, const int writePlace, threadInfo t_info) {

	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<float> d(0.0, 1.0);



	int subSamplesPerThread = 4;

	vec3 col(0.0f);

	for (int s = 0; s <subSamplesPerThread; s++) {

		float u = float(t_info.i + d(g)) / float(t_info.nx);
		float v = float(t_info.j + d(g)) / float(t_info.ny);
		Ray r = t_info.c.get_ray(u, v);

		col += color(r, t_info.world, 0, d, g);
	}

	col /= float(subSamplesPerThread);
	

	*colorList[writePlace] = col;
}

vec3 average_color(vec3** colorList, const int lenght) {
	
	vec3 sum = vec3(0.0f);
	for (int i = 0; i < lenght; i++) {
		sum += *colorList[i];
	}
	return sum / lenght;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------

int main()
{

	auto start = std::chrono::system_clock::now();

	int nx = 100;
	int ny = 50;
	int ns = 16;

	std::random_device randomDevice;
	std::mt19937 Generator(randomDevice());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);


	hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -2), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1.8, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1.4, 0, 0.0), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-2, 0, -2), 0.5, new Emmisive(vec3(0.5f,0.1f,0.02f)));
	hitable *world = new hitable_list(list, 5);


	vec3 lookfrom(-4.0f, 2.0f, +5.0f);
	vec3 looktat(0.0f, 0.2f, 0.0f);
	float dist_to_focus = (lookfrom - looktat).length();
	float aperture = 0.15f;

	//hitable *world = random_scene(distribution, Generator);
	camera c(lookfrom, looktat,  vec3(0.0f, 1.0f, 0.0f),60.0f, float(nx)/float( ny), aperture, dist_to_focus, distribution, Generator) ;


	std::ofstream file("output.ppm");

//-----------------------------------------------------------------------------------------------------------------------------------------------


	file << "P3\n" << nx << "  " << ny << "\n255\n";

	// 16 supported threads on my machine. I will do for now one sub sample per pixel, with 16 threads.
	int numberOfPossibleThread = std::thread::hardware_concurrency();

	std::thread t[16];

	int colListLenght = numberOfPossibleThread;
	vec3** colList = new vec3*[colListLenght];
	for (int i = 0; i < colListLenght; i++) {
		colList[i] = new vec3(distribution(Generator));
	//std::cout << *colList[i] << "\n";
	}

	threadInfo t_i(nx, ny, 0, 0, c, world);
	float counter = 0;
	for (int j = ny -1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			counter++;
			vec3 col(0.0f);
		
			t_i.i = i;
			t_i.j = j;
			for (int s = 1; s < numberOfPossibleThread; s++) {
				t[s] = std::thread(accumlate_color,  colList, s, t_i);
			}

			// running the same thing on the main thread
			accumlate_color(colList, 0, t_i);

			for (int s = 1; s < numberOfPossibleThread; s++) {
				t[s].join();
			}

			col = average_color(colList, colListLenght);

			// Old single thread code. This is now spread across several threads. 

			//for (int s = 0; s < ns; s++) {
			//	float u = float(i+ distribution(Generator)) / float(nx);
			//	float v = float(j+ distribution(Generator)) / float(ny);

			//	
			//	Ray r = c.get_ray(u, v);

			//	col += color(r, world, 0, distribution, Generator);
			//}

			//col /= float(ns);
	
			// gamma correction
			//if (col.x() > 1.0f) {

			//	int a = 0; 
			//}
			//col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			
			int ir = min(255, int(255.99 * col[0]));
			int ig = min(255,int(255.99 * col[1]));
			int ib = min(255,int(255.99 * col[2]));
			file << ir << " " << ig << " " << ib << "\n";

			
			float progession = counter * 100.0f / ((float)ny*(float)nx);
			if (fmod(progession, 5.f) == 0) {
				auto timeInBetween = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = timeInBetween - start;
				std::cout << "Progression at: " << progession << "%, in " << elapsed_seconds.count()<< " program time\n";
			}
			
		}
	} 


// -----------------------------------------------------------------------------------------------------------------------------------------------
	

	//delete[] colList;

	delete world;
	file.close();

	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	std::cout << "press any key to close to programm";
	std::cin.get();
}

