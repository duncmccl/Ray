
#ifndef __primitive_H
#define __primitive_H

#include <math.h>


typedef struct {
	double a, r, g, b;
} color_t;

typedef struct {
	double x, y, z;
} vec_t;

float vec_dot(vec_t A, vec_t B);

vec_t vec_cross(vec_t A, vec_t B);

vec_t vec_add(vec_t A, vec_t B);
vec_t vec_sub(vec_t A, vec_t B);

vec_t vec_scale(vec_t A, float s);

vec_t vec_rotate(vec_t A, vec_t B, float s);

typedef struct {
	vec_t *A, *B, *C;
} triangle_t;



typedef struct {
	vec_t *ori;
	float *radius;
} sphere_t;


typedef struct {
	vec_t * min;
	vec_t * max;
} cuboid_t;



enum primitive_type {TRIANGLE, SPHERE, CUBOID};

typedef struct {
	
	enum primitive_type type;
	void * data;
	
} primitive_t;


typedef struct {
	
	double dist;
	
	primitive_t primitive;
	
	vec_t intersection;
	
	vec_t normal;
	
	double u, v;
	
} intersect_t;

void cast_ray(const vec_t * RayOri, const vec_t * RayDir, const primitive_t * primitive, intersect_t * rtn);



typedef struct {
	vec_t pos;
	vec_t look;
	vec_t up;
	vec_t right;
	
	int hRES;
	int vRES;
	
	float hFOV;
	float vFOV;
} camera_t;


#endif