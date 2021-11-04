
#ifndef __PRIMATIVE_H
#define __PRIMATIVE_H

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
	vec_t *point;
	float radius;
} sphere_t;


typedef struct {
	vec_t * min;
	vec_t * max;
} cuboid_t;



enum primative_type {TRIANGLE, SPHERE, CUBOID};

typedef struct {
	
	enum primative_type type;
	void * data;
	
} primative_t;


typedef struct {
	
	double dist;
	
	primative_t primative;
	
	vec_t intersection;
	
	vec_t normal;
	
	double u, v;
	
} intersect_t;

void cast_ray(const vec_t * RayOri, const vec_t * RayDir, const primative_t * primative, intersect_t * rtn);



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