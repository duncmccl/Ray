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
	
	Camera.hRES = 7680;
	Camera.vRES = 4320;
	
	Camera.hFOV = M_PI / 2.0f;
	Camera.vFOV = Camera.hFOV * ((float)Camera.vRES / (float)Camera.hRES);
	
	
	unsigned int * PixelBuffer = (unsigned int *) malloc(sizeof(unsigned int) * Camera.hRES * Camera.vRES);
	
	
	// Make primitives list here!
	
	
	
	
	float rho = 1.0f;
	float theta = 0.0f;
	float phi = M_PI / 4.0f;
	
	
	vec_t phys_vec_list[4];
	int phys_vec_count = 4;
	
	phys_vec_list[0] = (vec_t){rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)};
	phys_vec_list[1] = (vec_t){rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
	phys_vec_list[2] = (vec_t){rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
	
	triangle_t triangle = (triangle_t){phys_vec_list + 0, phys_vec_list + 1, phys_vec_list + 2};
	
	
	float gamma = 0.0f;
	double radius = 0.5f;
	
	phys_vec_list[3] = (vec_t){0.0f, 1.0f * sin(gamma), 0.0f};
	
	sphere_t sphere = (sphere_t){phys_vec_list + 3, &radius};
	
	
	primitive_t phys_primitive_list[2];
	int phys_primitive_count = 2;
	
	phys_primitive_list[0].type = TRIANGLE;
	phys_primitive_list[0].data.triangle = triangle;
	
	phys_primitive_list[1].type = SPHERE;
	phys_primitive_list[1].data.sphere = sphere;
	
	
	
	model_t phys_model = (model_t){phys_vec_list, phys_vec_count, phys_primitive_list, phys_primitive_count};
	printf("Phys Model: %d, %d\n", phys_model.vec_count, phys_model.primitive_count);
	
	model_t * bunny = load_model("bunny.obj");
	printf("Bunny Model: %d, %d\n", bunny->vec_count, bunny->primitive_count);
	
	vec_t bunny_look = (vec_t){1.0, 0.0, 0.0};
	vec_t bunny_up = (vec_t){0.0, 1.0, 0.0};
	vec_t bunny_right = (vec_t){0.0, 0.0, 1.0};
	vec_t bunny_pos = (vec_t){0.0, 0.0, 0.0};

	
	
	
	
	
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

		phys_vec_list[0] = (vec_t){rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)};
		phys_vec_list[1] = (vec_t){rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		phys_vec_list[2] = (vec_t){rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		
		
		gamma += M_PI / 32.0f;
		phys_vec_list[3] = (vec_t){0.0f, 1.0f * sin(gamma), 0.0f};
		
		
		
		
		bunny_pos.z += 0.01;
		bunny_pos.y -= 0.01;
		
		
		
		model_t * moving_bunny = copy_model(bunny);
		trans_rotate_model(moving_bunny, &bunny_look, &bunny_up, &bunny_right, &bunny_pos);
		
		
		// Render
		
		int model_count = 2;
		model_t * model_list[2] = {&phys_model, moving_bunny};
		
		model_t * render_target = aggregate_models(model_list, model_count);
		bvh_t * bvh = build_bvh(render_target, 16, 8);
		
		render_image(&Camera, bvh, PixelBuffer);
		
		destroy_bvh(bvh);
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