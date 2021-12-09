
#ifndef __primitive_H
#define __primitive_H


typedef struct {
	float a, r, g, b;
} color_t;

typedef struct {
	float x, y, z;
} vec_t;

typedef struct {
	vec_t i;
	vec_t j;
	vec_t k;
	vec_t l;
} transform_t;

float vec_dot(vec_t A, vec_t B);

vec_t vec_cross(vec_t A, vec_t B);

vec_t vec_add(vec_t A, vec_t B);
vec_t vec_sub(vec_t A, vec_t B);

vec_t vec_scale(vec_t A, float s);

vec_t vec_rotate(vec_t A, vec_t B, float s);


typedef struct {
	int A, B, C;
} triangle_t;


typedef struct {
	int origin;
	int radius;
} ellipsoid_t;


typedef struct {
	int min;
	int max;
} cuboid_t;



enum primitive_type {INVALID, TRIANGLE, ELLIPSOID, CUBOID};

typedef struct {
	
	enum primitive_type type;
	
	union primitive_data {
		
		triangle_t triangle;
		ellipsoid_t ellipsoid;
		cuboid_t cuboid;
		
	} data;
	
} primitive_t;




typedef struct {
	
	vec_t * vec_list;
	int vec_count;
	
	primitive_t * primitive_list;
	int primitive_count;
	
	transform_t basis;
	
} model_t;

model_t * load_model(const char * fname);
model_t * aggregate_models(model_t ** model_list, int model_count);
model_t * copy_model(model_t * target);
void transform_model(model_t * target);
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
	transform_t basis;
	
	int hRES;
	int vRES;
	
	float hFOV;
	float vFOV;
} camera_t;


#endif