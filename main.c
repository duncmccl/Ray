// This is to be used as a template

// Any further questions can be directed to my email:
//		duncmccl@udel.edu

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "bitmap.h"
#include "primitive.h"
#include "Ray.h"



int main() {
	
	
	// Init
	
	camera_t Camera;
	Camera.pos =	(vec_t){-5.0f, 0.0f, 0.0f};
	Camera.look =	(vec_t){1.0f, 0.0f, 0.0f};
	Camera.up =		(vec_t){0.0f, 1.0f, 0.0f};
	Camera.right =	(vec_t){0.0f, 0.0f, 1.0f};
	
	Camera.hRES = 640;
	Camera.vRES = 360;
	
	Camera.hFOV = M_PI / 2.0f;
	Camera.vFOV = Camera.hFOV * ((float)Camera.vRES / (float)Camera.hRES);
	
	
	unsigned int * PixelBuffer = (unsigned int *) malloc(sizeof(unsigned int) * Camera.hRES * Camera.vRES);
	
	
	// Make primitives list here!
	
	/*
	
	primitive are an ENUM and a pointer to the actual data.
	This is done to allow for a single list of multiple diffrent things,
	such as triangles, spheres, cuboids, etc.
	
	*/
	
	
	float rho = 1.0f;
	float theta = 0.0f;
	float phi = M_PI / 4.0f;
	
	vec_t A = (vec_t){rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)};
	vec_t B = (vec_t){rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
	vec_t C = (vec_t){rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
	
	triangle_t triangle = (triangle_t){&A, &B, &C};
	
	
	float gamma = 0.0f;
	float radius = 0.5f;
	
	vec_t D = (vec_t){0.0f, 1.0f * sin(gamma), 0.0f};
	
	sphere_t sphere = (sphere_t){&D, &radius};
	
	
	primitive_t primitive_list[] = {
		(primitive_t){TRIANGLE, &triangle},
		(primitive_t){SPHERE, &sphere}};
	
	unsigned long primitive_count = 2;
	
	
	
	// Render Loop
	
	char loop = 1;
	int frame_count = 0;
	int frame_limit = 10;
	
	char title[64];
	
	mkdir("./Output", 0777);
	
	
	while (loop) {
		
		// PHYSICS OR ANYTHING ELSE GOES HERE:
		theta += M_PI / 64.0f;
		phi += M_PI / 128.0f;

		A = (vec_t){rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)};
		B = (vec_t){rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		C = (vec_t){rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		
		
		gamma += M_PI / 32.0f;
		D = (vec_t){0.0f, 1.0f * sin(gamma), 0.0f};
		
		
		
		// Render
		
		
		render_image(&Camera, primitive_list, primitive_count, PixelBuffer);
		
		
		sprintf(title, "./Output/Frame_%d.bmp", frame_count);
		save_bmp((unsigned char *)PixelBuffer, Camera.hRES, Camera.vRES, title);
		
		
		frame_count++;
		if (frame_count >= frame_limit) loop = 0;
	}
	
	
	// Cleanup
	free(PixelBuffer);
	
	
	return 0;
}