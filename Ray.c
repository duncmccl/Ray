#include "Ray.h"


char cast_ray_triangle(const vec_t * RayOri, const vec_t * RayDir, const triangle_t * Tri, intersect_t * rtn) {
	vec_t * A = Tri->A;
	vec_t * B = Tri->B;
	vec_t * C = Tri->C;
	
	vec_t AB = vec_sub(*B, *A);
	vec_t AC = vec_sub(*C, *A);
	
	vec_t norm = vec_cross(AB, AC);
	
	norm = vec_scale(norm, 1.0f / sqrt(vec_dot(norm, norm)));
	
	float i = vec_dot(*RayDir, norm);
	
	// No hit, Ray is parallel to triangle.
	if (i == 0.0) return 0;
	
	// Wrong side of the Triangle
	//if (i > 0.0) return 0; 
	
	float t = vec_dot(vec_sub(*A, *RayOri), norm) / i;
	
	if (t < 0.0f || t > rtn->dist) {
		return 0;
	}
	
	vec_t X = vec_add(*RayOri, vec_scale(*RayDir, t));
	vec_t dX = vec_sub(X, *A);
	
	float d00 = vec_dot(AB, AB);
	float d01 = vec_dot(AB, AC);
	float d02 = vec_dot(AB, dX);
	float d11 = vec_dot(AC, AC);
	float d12 = vec_dot(AC, dX);
	
	float invDenom = 1.0 / ((d01 * d01) - (d00 * d11));
	
	float u = ((d12 * d01) - (d02 * d11)) * invDenom;
	float v = ((d02 * d01) - (d12 * d00)) * invDenom;
	
	if (u < 0.0f || v < 0.0f || u + v > 1.0f) {
		return 0;
	}
	
	if (rtn->dist > t) {
		rtn->dist = t;
		rtn->intersection = X;
		rtn->normal = norm;
		
		rtn->u = u;
		rtn->v = v;
		
		return 1;
	}
	
	return 0;
}


char cast_ray_sphere(const vec_t * RayOri, const vec_t * RayDir, const sphere_t * Tri, intersect_t * rtn) {
	
	return 0;
}

char cast_ray_cuboid(const vec_t * RayOri, const vec_t * RayDir, const cuboid_t * Tri, intersect_t * rtn) {
	
	return 0;
}


void trace_ray(const vec_t * RayOri, const vec_t * RayDir, const primative_t * primative_list, const int primative_count, color_t * rtn) {
	
	
	intersect_t I;
	I.dist = INFINITY;
	
	for(int i = 0; i < primative_count; i++) {
		
		switch (primative_list[i].type) {
			case TRIANGLE:
				if (cast_ray_triangle(RayOri, RayDir, (triangle_t *)primative_list[i].data, &I)) {
					I.primative = primative_list[i];
				}
				break;
				
			case SPHERE:
				if (cast_ray_sphere(RayOri, RayDir, (sphere_t *)primative_list[i].data, &I)) {
					I.primative = primative_list[i];
				}
				break;
				
			case CUBOID:
				if (cast_ray_cuboid(RayOri, RayDir, (cuboid_t *)primative_list[i].data, &I)) {
					I.primative = primative_list[i];
				}
				break;
				
			default:
				break;
				
		}
		
	}
	
	
	if (I.dist < INFINITY) {
		
		float s = vec_dot(I.normal, *RayDir);
		s = abs(s);
		
		*rtn = (color_t){1.0f, s, s, s};
		
	} else {
		*rtn = (color_t){0.0f, 0.0f, 0.0f, 0.0f};
	}
	
}



unsigned int color_to_int(color_t color) {
	
	// A B G R
	
	return (((unsigned int)floor(color.a * 255.0f) & 0xFF) << 24) | 
		   (((unsigned int)floor(color.b * 255.0f) & 0xFF) << 16) | 
		   (((unsigned int)floor(color.g * 255.0f) & 0xFF) << 8) | 
		   (((unsigned int)floor(color.r * 255.0f) & 0xFF) << 0);
	
}

void render_image(const camera_t * Camera, const primative_t * primative_list, const unsigned long primative_count, unsigned int * PixelBuffer) {
	
	for(int y = 0; y < Camera->vRES; y++) {
		for(int x = 0; x < Camera->hRES; x++) {
			
			float Pitch = (Camera->vFOV / 2.0f) - (Camera->vFOV / (float)Camera->vRES) * y;
			float Yaw = (Camera->hFOV / 2.0f) - (Camera->hFOV / (float)Camera->hRES) * x;
			
			vec_t axis = vec_add(vec_scale(Camera->up, Yaw), vec_scale(Camera->right, Pitch));
			float im = 1.0f / sqrt(vec_dot(axis, axis));
			axis = vec_scale(axis, im);
			
			vec_t DIR = vec_rotate(Camera->look, axis, sqrt(Yaw*Yaw + Pitch*Pitch));
			
			color_t color = (color_t){0.0, 0.0, 0.0, 0.0};
			trace_ray(&Camera->pos, &DIR, primative_list, primative_count, &color);
			
			PixelBuffer[x + y * Camera->hRES] = color_to_int(color);
			
		}
	}
}
