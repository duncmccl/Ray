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
	Camera.basis.i = (vec_t){1.0f, 0.0f, 0.0f};
	Camera.basis.j = (vec_t){0.0f, 1.0f, 0.0f};
	Camera.basis.k = (vec_t){0.0f, 0.0f, 1.0f};
	Camera.basis.l = (vec_t){-5.0f, 0.0f, 0.0f};
	
	Camera.hRES = 1920;
	Camera.vRES = 1080;
	
	Camera.hFOV = M_PI / 2.0f;
	Camera.vFOV = Camera.hFOV * ((float)Camera.vRES / (float)Camera.hRES);
	
	
	unsigned int * PixelBuffer = (unsigned int *) malloc(sizeof(unsigned int) * Camera.hRES * Camera.vRES);
	
	
	// Make primitives list here!
	
	
	
	
	float rho = 1.0f;
	float theta = 0.0f;
	float phi = M_PI / 4.0f;
	
	
	vec_t phys_vec_list[5];
	int phys_vec_count = 5;
	
	phys_vec_list[0] = (vec_t){(float)(rho * sin(phi) * cos(theta)), 
							   (float)(rho * sin(phi) * sin(theta)), 
							   (float)(rho * cos(phi))};
	phys_vec_list[1] = (vec_t){(float)(rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f))), 
							   (float)(rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f))), 
							   (float)(rho * cos(phi - M_PI))};
	phys_vec_list[2] = (vec_t){(float)(rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f))), 
							   (float)(rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f))), 
							   (float)(rho * cos(phi - M_PI))};
	
	triangle_t triangle = (triangle_t){ 0, 1, 2};
	
	
	float gamma = 0.0f;
	
	phys_vec_list[3] = (vec_t){-0.25f, 1.0f * sin(gamma), 0.0f};
	phys_vec_list[4] = (vec_t){0.4f, 0.1f, 0.2f};
	
	ellipsoid_t ellipsoid = (ellipsoid_t){3, 4};
	
	
	primitive_t phys_primitive_list[2];
	int phys_primitive_count = 2;
	
	phys_primitive_list[0].type = TRIANGLE;
	phys_primitive_list[0].data.triangle = triangle;
	
	phys_primitive_list[1].type = ELLIPSOID;
	phys_primitive_list[1].data.ellipsoid = ellipsoid;
	
	
	
	model_t phys_model;
	phys_model.vec_list = phys_vec_list;
	phys_model.vec_count = phys_vec_count;
	phys_model.primitive_list = phys_primitive_list;
	phys_model.primitive_count = phys_primitive_count;
	
	phys_model.basis.i = (vec_t){1.0f, 0.0f, 0.0f};
	phys_model.basis.j = (vec_t){0.0f, 1.0f, 0.0f};
	phys_model.basis.k = (vec_t){0.0f, 0.0f, 1.0f};
	phys_model.basis.l = (vec_t){0.0f, 0.0f, 0.0f};
	
	printf("Phys Model: %d, %d\n", phys_model.vec_count, phys_model.primitive_count);
	
	model_t * bunny = load_model("bunny.obj");
	printf("Bunny Model: %d, %d\n", bunny->vec_count, bunny->primitive_count);
	
	bunny->basis.i = (vec_t){1.0f, 0.0f, 0.0f};
	bunny->basis.j = (vec_t){0.0f, 1.0f, 0.0f};
	bunny->basis.k = (vec_t){0.0f, 0.0f, 1.0f};
	bunny->basis.l = (vec_t){0.0f, 0.0f, 0.0f};
	
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
	
	
	
	// Render Loop
	
	char loop = 1;
	int frame_count = 0;
	int frame_limit = 30;
	
	char title[64];
	
	mkdir("./Output", 0777);
	
	
	while (loop) {
		
		printf("Frame: %d\n", frame_count);
		
		// PHYSICS OR ANYTHING ELSE GOES HERE:
		theta += M_PI / 64.0f;
		phi += M_PI / 128.0f;

		phys_vec_list[0] = (vec_t){(float)(rho * sin(phi) * cos(theta)), 
								   (float)(rho * sin(phi) * sin(theta)), 
								   (float)(rho * cos(phi))};
		phys_vec_list[1] = (vec_t){(float)(rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f))), 
								   (float)(rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f))), 
								   (float)(rho * cos(phi - M_PI))};
		phys_vec_list[2] = (vec_t){(float)(rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f))), 
								   (float)(rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f))), 
								   (float)(rho * cos(phi - M_PI))};
		
		
		gamma += M_PI / 32.0f;
		phys_vec_list[3] = (vec_t){-0.25f, 1.0f * sin(gamma), 0.0f};
		
		
		bunny->basis.i = (vec_t){(cos(pitch)*cos(yaw)), ((cos(pitch)*sin(yaw)*sin(roll)) - (sin(pitch)*cos(roll))), ((cos(pitch)*sin(yaw)*cos(roll)) + (sin(pitch)*sin(roll)))};
		bunny->basis.j = (vec_t){(sin(pitch)*cos(yaw)), ((sin(pitch)*sin(yaw)*sin(roll)) + (cos(pitch)*cos(roll))), ((sin(pitch)*sin(yaw)*cos(roll)) - (cos(pitch)*sin(roll)))};
		bunny->basis.k = (vec_t){(-sin(yaw)), (cos(yaw)*sin(roll)), (cos(yaw)*cos(roll))};
		
		bunny->basis.l.x += 0.1f;
		bunny->basis.l.y += 0.2f;
		bunny->basis.l.z += 0.4f;
		
		
		
		// Render
		
		int model_count = 2;
		model_t * model_list[2] = {&phys_model, bunny};
		
		
		render_image(&Camera, model_list, model_count, PixelBuffer);
		
		sprintf(title, "./Output/Frame_%d.bmp", frame_count);
		save_bmp((unsigned char *)PixelBuffer, Camera.hRES, Camera.vRES, title);
		
		
		frame_count++;
		if (frame_count >= frame_limit) loop = 0;
		
		yaw += 0.01f;
		pitch -= 0.2f;
		roll += 0.05f;
	}
	
	
	// Cleanup
	free(PixelBuffer);
	
	
	destroy_model(bunny);
	
	return 0;
}