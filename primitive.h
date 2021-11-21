
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
	double *radius;
} sphere_t;


typedef struct {
	vec_t * min;
	vec_t * max;
} cuboid_t;



enum primitive_type {INVALID, TRIANGLE, SPHERE, CUBOID};

typedef struct {
	
	enum primitive_type type;
	
	union primitive_data {
		
		triangle_t triangle;
		sphere_t sphere;
		cuboid_t cuboid;
		
	} data;
	
} primitive_t;




typedef struct {
	
	vec_t * vec_list;
	int vec_count;
	
	primitive_t * primitive_list;
	int primitive_count;
	
} model_t;

model_t * load_model(const char * fname);
model_t * aggregate_models(model_t ** model_list, int model_count);
model_t * copy_model(model_t * target);
void trans_rotate_model(model_t * target, vec_t * look, vec_t * up, vec_t * right, vec_t * pos);
void destroy_model(model_t * model);



typedef struct __bvh {
	
	vec_t min;
	vec_t mid;
	vec_t max;
	
	struct __bvh * children[8];
	int children_count;
	
	primitive_t * primitive_list;
	int primitive_count;
	
} bvh_t;

bvh_t * build_bvh(model_t * model, int limit, int depth);
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