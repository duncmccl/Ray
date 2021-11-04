#include "Ray.h"

#include <stdio.h>

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


char cast_ray_sphere(const vec_t * RayOri, const vec_t * RayDir, const sphere_t * Sph, intersect_t * rtn) {
	/*
	
	Ray: origin, direction
		Ro, Rd
	
	Sphere: origin, radius
		So, Sr
	
	X = Ro + Rd * t
	
	(X - So) * (X - So) = Sr * Sr
	
	((Ro - So) + Rd * t) * ((Ro - So) + Rd * t) = Sr * Sr
	
	(Ro - So)*(Ro - So) + 2 * (Ro - So) * Rd * t + Rd * Rd * t * t = Sr * Sr
	
	C = (Ro - So) * (Ro - So) - (Sr * Sr)
	B = 2 * (Ro - So) * Rd
	A = Rd * Rd
	
	At^2 + Bt + C = 0
	
	t = (-b+-sqrt(b^2 - 4ac))/2a
	
	*/
	
	vec_t v0 = vec_sub(*RayOri, (*Sph->ori));
	
	float C = vec_dot(v0, v0) - ((*Sph->radius) * (*Sph->radius));
	float B = 2.0f * vec_dot(v0, *RayDir);
	float A = vec_dot(*RayDir, *RayDir);
	
	
	
	if ((B * B) > (4.0f * A * C)) {
		
		float t1 = (-B + sqrt((B * B) - (4.0f * A * C))) / (2.0f * A);
		float t2 = (-B - sqrt((B * B) - (4.0f * A * C))) / (2.0f * A);
		
		float t = (t1 > 0.0f ? (t2 > 0.0f ? (t1 < t2 ? t1 : t2) : t1) : (t2 > 0.0f ? t2 : 0.0f));
		
		if (rtn->dist > t) {
			
			vec_t X = vec_add(*RayOri, vec_scale(*RayDir, t));
			vec_t norm = vec_sub(X, *Sph->ori);
			norm = vec_scale(norm, 1.0f / *Sph->radius);
			
			float u = (atan(norm.y / norm.x) + M_PI/2.0f) / M_PI;
			float v = (atan(norm.z / sqrt((norm.x * norm.x) + (norm.y * norm.y))) + M_PI/2.0f) / M_PI;
			
			rtn->dist = t;
			rtn->intersection = X;
			rtn->normal = norm;

			rtn->u = u;
			rtn->v = v;

			return 1;
		}
	}
	
	
	
	return 0;
}

char cast_ray_cuboid(const vec_t * RayOri, const vec_t * RayDir, const cuboid_t * Tri, intersect_t * rtn) {
	
	return 0;
}


void trace_ray(const vec_t * RayOri, const vec_t * RayDir, const primitive_t * primitive_list, const int primitive_count, color_t * rtn) {
	
	
	intersect_t I;
	I.dist = INFINITY;
	
	for(int i = 0; i < primitive_count; i++) {
		
		switch (primitive_list[i].type) {
			case TRIANGLE:
				if (cast_ray_triangle(RayOri, RayDir, (triangle_t *)primitive_list[i].data, &I)) {
					I.primitive = primitive_list[i];
				}
				break;
				
			case SPHERE:
				if (cast_ray_sphere(RayOri, RayDir, (sphere_t *)primitive_list[i].data, &I)) {
					I.primitive = primitive_list[i];
				}
				break;
				
			case CUBOID:
				if (cast_ray_cuboid(RayOri, RayDir, (cuboid_t *)primitive_list[i].data, &I)) {
					I.primitive = primitive_list[i];
				}
				break;
				
			default:
				break;
				
		}
		
	}
	
	
	if (I.dist < INFINITY) {
		
		float s = vec_dot(I.normal, *RayDir);
		s = abs(s);
		
		float r = (1.0f - I.u) * (1.0f - I.v);
		float g = (I.u) * (1.0f - I.v);
		float b = (1.0f - I.u) * (I.v);
		
		
		*rtn = (color_t){1.0f, s * r, s * g, s * b};
		
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

void render_image(const camera_t * Camera, const primitive_t * primitive_list, const unsigned long primitive_count, unsigned int * PixelBuffer) {
	
	#pragma omp parallel for collapse(2)
	for(int y = 0; y < Camera->vRES; y++) {
		for(int x = 0; x < Camera->hRES; x++) {
			
			float Pitch = (Camera->vFOV / 2.0f) - (Camera->vFOV / (float)Camera->vRES) * y;
			float Yaw = (Camera->hFOV / 2.0f) - (Camera->hFOV / (float)Camera->hRES) * x;
			
			vec_t axis = vec_add(vec_scale(Camera->up, Yaw), vec_scale(Camera->right, Pitch));
			float im = 1.0f / sqrt(vec_dot(axis, axis));
			axis = vec_scale(axis, im);
			
			vec_t DIR = vec_rotate(Camera->look, axis, sqrt(Yaw*Yaw + Pitch*Pitch));
			
			color_t color = (color_t){0.0, 0.0, 0.0, 0.0};
			trace_ray(&Camera->pos, &DIR, primitive_list, primitive_count, &color);
			
			PixelBuffer[x + y * Camera->hRES] = color_to_int(color);
			
		}
	}
}
