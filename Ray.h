
#ifndef __RAY_H
#define __RAY_H

#include "primitive.h"



// Intersection structure that describes an intersection
// between a ray and a primitive.
typedef struct {
	
	double dist;
	
	primitive_t * primitive;
	
	vec_t intersection;
	
	vec_t normal;
	
	double u, v;
	
} intersect_t;

int cast_ray_triangle(vec_t * ray_ori, vec_t * ray_dir, triangle_t * tri, intersect_t * rtn);
int cast_ray_ellipsoid(vec_t * ray_ori, vec_t * ray_dir, ellipsoid_t * ellipsoid, intersect_t * rtn);
int cast_ray_cuboid(vec_t * ray_ori, vec_t * ray_dir, cuboid_t * cuboid, intersect_t * rtn);
int cast_ray_primitive(vec_t * ray_ori, vec_t * ray_dir, primitive_t * primitive, intersect_t * rtn);
int cast_ray_bvh(vec_t * ray_ori, vec_t * ray_dir, bvh_t * bvh, intersect_t * rtn);


void render_image(camera_t * Camera, model_t * render_target, unsigned int * PixelBuffer);

#endif