
#ifndef __RAY_H
#define __RAY_H

#include "primitive.h"

typedef struct {
	
	double dist;
	
	primitive_t * primitive;
	
	vec_t intersection;
	
	vec_t normal;
	
	double u, v;
	
} intersect_t;

int cast_ray_triangle(vec_t * RayOri, vec_t * RayDir, triangle_t * tri, intersect_t * rtn);
int cast_ray_sphere(vec_t * RayOri, vec_t * RayDir, sphere_t * sphere, intersect_t * rtn);
int cast_ray_cuboid(vec_t * RayOri, vec_t * RayDir, cuboid_t * cuboid, intersect_t * rtn);
int cast_ray_primitive(vec_t * RayOri, vec_t * RayDir, primitive_t * primitive, intersect_t * rtn);
int cast_ray_bvh(vec_t * RayOri, vec_t * RayDir, bvh_t * bvh, intersect_t * rtn);



void render_image(camera_t * Camera, bvh_t * bvh, unsigned int * PixelBuffer);

#endif