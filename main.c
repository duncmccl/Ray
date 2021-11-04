// This is to be used as a template

// I will assume that you (the reader of this) is able to install and link SDL properly.
// If you can't, find a tutorial online.

// Any further questions can be directed to my email:
//		duncmccl@udel.edu

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <SDL2/SDL.h>

#include "primitive.h"
#include "Ray.h"

int main() {
	
	
	// Init
	
	camera_t Camera;
	Camera.pos =	(vec_t){-5.0f, 0.0f, 0.0f};
	Camera.look =	(vec_t){1.0f, 0.0f, 0.0f};
	Camera.up =		(vec_t){0.0f, 1.0f, 0.0f};
	Camera.right =	(vec_t){0.0f, 0.0f, 1.0f};
	
	Camera.hRES = 512;
	Camera.vRES = 512;
	
	Camera.hFOV = M_PI / 2.0f;
	Camera.vFOV = M_PI / 2.0f;
	
	
	int window_width = 512;
	int window_height = 512;
	
	float moveSpeed = 0.1f;
	float lookSpeed = M_PI / 32.0f;
	
	
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
	
	
	
	
	
	
	
	
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	// Blending for image scaling
	// 0	Nearest pixel
	// 1	Linear filtering
	// 2	Anisotropic Filtering
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	
	
	SDL_Window * MainWindow = SDL_CreateWindow("WINDOW TITLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
	if (!MainWindow) {
		printf("Failed to create SDL Window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	unsigned int render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer * MainRenderer = SDL_CreateRenderer(MainWindow, -1, render_flags);
	if (!MainRenderer) {
		printf("Failed to create Renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(MainWindow);
		SDL_Quit();
		return 1;
	}
	
	SDL_Texture * TextureBuffer = SDL_CreateTexture(MainRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Camera.hRES, Camera.vRES);
	SDL_SetTextureBlendMode(TextureBuffer, SDL_BLENDMODE_NONE);
	
	
	
	// Render Loop
	
	char loop = 1;
	
	unsigned long look_up		(1 << 0);
	unsigned long look_down		(1 << 1);
	unsigned long look_right	(1 << 2);
	unsigned long look_left		(1 << 3);
	unsigned long roll_clock	(1 << 4);
	unsigned long roll_counter	(1 << 5);
	
	unsigned long move_up		(1 << 6);
	unsigned long move_down		(1 << 7);
	unsigned long move_forward	(1 << 8);
	unsigned long move_back		(1 << 9);
	unsigned long move_left		(1 << 10);
	unsigned long move_right	(1 << 11);
	
	unsigned long user_input = 0;
	
	while (loop) {
		
		// Input handling
		
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
				case SDL_QUIT:
					loop = 0;
					break;
				case SDL_KEYDOWN:
					
					switch (evt.key.keysym.sym) {
						case SDLK_LEFT:
							user_input |= look_left;
							break;
						case SDLK_RIGHT:
							user_input |= look_right;
							break;
						case SDLK_UP:
							user_input |= look_up;
							break;
						case SDLK_DOWN:
							user_input |= look_down;
							break;
						case SDLK_e:
							user_input |= roll_clock;
							break;
						case SDLK_q:
							user_input |= roll_counter;
							break;
							
						case SDLK_w:
							user_input |= move_forward;
							break;
						case SDLK_a:
							user_input |= move_left;
							break;
						case SDLK_s:
							user_input |= move_back;
							break;
						case SDLK_d:
							user_input |= move_right;
							break;
						case SDLK_SPACE:
							user_input |= move_up;
							break;
						case SDLK_LSHIFT:
							user_input |= move_down;
							break;
							
						default:
							break;
					}
					
					break;
				case SDL_KEYUP:
					
					switch(evt.key.keysym.sym) {
						case SDLK_LEFT:
							user_input &= !look_left;
							break;
						case SDLK_RIGHT:
							user_input &= !look_right;
							break;
						case SDLK_UP:
							user_input &= !look_up;
							break;
						case SDLK_DOWN:
							user_input &= !look_down;
							break;
						case SDLK_e:
							user_input &= !roll_clock;
							break;
						case SDLK_q:
							user_input &= !roll_counter;
							break;
							
						case SDLK_w:
							user_input &= !move_forward;
							break;
						case SDLK_a:
							user_input &= !move_left;
							break;
						case SDLK_s:
							user_input &= !move_back;
							break;
						case SDLK_d:
							user_input &= !move_right;
							break;
						case SDLK_SPACE:
							user_input &= !move_up;
							break;
						case SDLK_LSHIFT:
							user_input &= !move_down;
							break;
							
						default:
							break;
					}
					
					break;
				default:
					break;
			}
		}
		
		
		// Move and pan camera around based on user input:
		vec_t axis = (vec_t){0.0f, 0.0f, 0.0f};
		if (user_input & look_up) {
			axis = vec_add(axis, Camera.right);
		}
		if (user_input & look_down) {
			axis = vec_sub(axis, Camera.right);
		}
		if (user_input & look_left) {
			axis = vec_add(axis, Camera.up);
		}
		if (user_input & look_right) {
			axis = vec_sub(axis, Camera.up);
		}
		if (user_input & roll_clock) {
			axis = vec_add(axis, Camera.look);
		}
		if (user_input & roll_counter) {
			axis = vec_sub(axis, Camera.look);
		}
		float axis_mag = vec_dot(axis, axis);
		if (axis_mag > 0.001f) {
			axis = vec_scale(axis, 1.0f / sqrt(axis_mag));
			
			Camera.look = vec_rotate(Camera.look, axis, lookSpeed);
			Camera.up = vec_rotate(Camera.up, axis, lookSpeed);
			Camera.right = vec_rotate(Camera.right, axis, lookSpeed);
			
		}
		
		vec_t move = (vec_t){0.0f, 0.0f, 0.0f};
		if (user_input & move_forward) {
			move = vec_add(move, Camera.look);
		}
		if (user_input & move_back) {
			move = vec_sub(move, Camera.look);
		}
		if (user_input & move_right) {
			move = vec_add(move, Camera.right);
		}
		if (user_input & move_left) {
			move = vec_sub(move, Camera.right);
		}
		if (user_input & move_up) {
			move = vec_add(move, Camera.up);
		}
		if (user_input & move_down) {
			move = vec_sub(move, Camera.up);
		}
		float move_mag = vec_dot(move, move);
		if (move_mag > 0.001f) {
			move = vec_scale(move, moveSpeed / sqrt(move_mag));
			
			Camera.pos = vec_add(Camera.pos, move);
			
		}
		
		
		
		
		
		// PHYSICS OR ANYTHING ELSE GOES HERE:
		theta += M_PI / 64.0f;
		phi += M_PI / 128.0f;

		A = (vec_t){rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)};
		B = (vec_t){rho * sin(phi) * cos(theta + (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta + (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		C = (vec_t){rho * sin(phi) * cos(theta - (2.0f * M_PI / 3.0f)), rho * sin(phi - M_PI) * sin(theta - (2.0f * M_PI / 3.0f)), rho * cos(phi - M_PI)};
		
		
		gamma += M_PI / 32.0f;
		D = (vec_t){0.0f, 1.0f * sin(gamma), 0.0f};
		
		
		
		
		
		
		
		
		// Render
		
		unsigned int * PixelBuffer;
		int Pitch;
		SDL_LockTexture(TextureBuffer, NULL, (void**)&PixelBuffer, &Pitch);
		
		// Clear Screen, Optional
		#pragma omp parallel for
		for(int z = 0; z < Camera.hRES * Camera.vRES; z++) {
			PixelBuffer[z] = 0;
		}
		
		render_image(&Camera, primitive_list, primitive_count, PixelBuffer);
		
		SDL_UnlockTexture(TextureBuffer);
		
		
		
		// Update screen
		SDL_RenderCopy(MainRenderer, TextureBuffer, NULL, NULL);
		SDL_RenderPresent(MainRenderer);
		
		
		
	}
	
	
	
	
	
	// Cleanup
	SDL_DestroyTexture(TextureBuffer);
	SDL_DestroyRenderer(MainRenderer);
	SDL_DestroyWindow(MainWindow);
	SDL_Quit();
	
	
	// Destroy primitive list here and things...
	
	
	
	
	
	
	return 0;
}