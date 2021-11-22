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
	
	
	
	model_t phys_model = (model_t){phys_vec_list, phys_vec_count, phys_primitive_list, phys_primitive_count};
	printf("Phys Model: %d, %d\n", phys_model.vec_count, phys_model.primitive_count);
	
	model_t * bunny = load_model("bunny.obj");
	printf("Bunny Model: %d, %d\n", bunny->vec_count, bunny->primitive_count);
	
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
	
	vec_t bunny_look = (vec_t){
		(float)(cos(yaw)*cos(pitch)),
		(float)(sin(yaw)*cos(pitch)),
		(float)(-sin(pitch))
		};
	vec_t bunny_up = (vec_t){
		(float)(cos(yaw)*sin(pitch)*sin(roll) - sin(yaw)*cos(roll)),
		(float)(sin(yaw)*sin(pitch)*cos(roll) + cos(yaw)*cos(roll)),
		(float)(cos(pitch)*sin(roll))
		};
	vec_t bunny_right = (vec_t){
		(float)(cos(yaw)*sin(pitch)*cos(roll) + sin(yaw)*sin(roll)),
		(float)(sin(yaw)*sin(pitch)*cos(roll) - cos(yaw)*sin(roll)),
		(float)(cos(pitch)*cos(roll))
		};
	vec_t bunny_pos = (vec_t){0.0f, 0.0f, 0.0f};

	
	
	
	
	
	// Render Loop
	
	char loop = 1;
	int frame_count = 0;
	int frame_limit = 60;
	
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
		
		
		
		
		yaw += 0.0f;	// Z Axis
		pitch += M_PI / 32.0f;			// Y axis
		roll += 0.0f;			// X axis

		bunny_look = (vec_t){
			(float)(cos(yaw)*cos(pitch)),
			(float)(sin(yaw)*cos(pitch)),
			(float)(-sin(pitch))
			};
		bunny_up = (vec_t){
			(float)(cos(yaw)*sin(pitch)*sin(roll) - sin(yaw)*cos(roll)),
			(float)(sin(yaw)*sin(pitch)*cos(roll) + cos(yaw)*cos(roll)),
			(float)(cos(pitch)*sin(roll))
			};
		bunny_right = (vec_t){
			(float)(cos(yaw)*sin(pitch)*cos(roll) + sin(yaw)*sin(roll)),
			(float)(sin(yaw)*sin(pitch)*cos(roll) - cos(yaw)*sin(roll)),
			(float)(cos(pitch)*cos(roll))
			};
		
		bunny_pos.x += 0.00f;
		bunny_pos.z += 0.01f;
		bunny_pos.y -= 0.01f;
		
		
		model_t * moving_bunny = copy_model(bunny);
		trans_rotate_model(moving_bunny, &bunny_look, &bunny_up, &bunny_right, &bunny_pos);
		
		
		// Render
		
		int model_count = 2;
		model_t * model_list[2] = {&phys_model, moving_bunny};
		
		model_t * render_target = aggregate_models(model_list, model_count);
		
		render_image(&Camera, render_target, PixelBuffer);
		
		destroy_model(render_target);
		destroy_model(moving_bunny);
		
		sprintf(title, "./Output/Frame_%d.bmp", frame_count);
		save_bmp((unsigned char *)PixelBuffer, Camera.hRES, Camera.vRES, title);
		
		
		frame_count++;
		if (frame_count >= frame_limit) loop = 0;
	}
	
	
	// Cleanup
	free(PixelBuffer);
	
	
	destroy_model(bunny);
	
	return 0;
}