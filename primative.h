
#ifndef __PRIMATIVE_H
#define __PRIMATIVE_H


typedef struct {
	float x, y, z;
} vec_t;

typedef struct {
	vec_t A, B, C;
} triangle_t;

typedef struct {
	vec_t point;
	float radius;
} sphere_t;

#endif