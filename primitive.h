
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
	
	vec_t * vec_list;
	
	primitive_t * primitive_list;
	int primitive_count;
	
} model_t;

model_t * load_model(const char * fname);
void destroy_model(model_t * model);



typedef struct __bvh {
	
	vec_t min;
	vec_t mid;
	vec_t max;
	
	struct __bvh * children[8];
	int children_count;
	
	primitive_t * primative_list;
	int primative_count;
	
} bvh_t;

bvh_t * build_bvh(model_t * model);
void destroy_bvh(bvh_t * bvh);




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