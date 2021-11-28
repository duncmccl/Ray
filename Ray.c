#include <stdio.h>
#include <math.h>

#include "Ray.h"



int cast_ray_triangle(vec_t * ray_ori, vec_t * ray_dir, triangle_t * triangle, vec_t * vec_list, intersect_t * rtn) {
	vec_t * A = vec_list + triangle->A;
	vec_t * B = vec_list + triangle->B;
	vec_t * C = vec_list + triangle->C;
	
	// Legs of the triangle
	vec_t AB = vec_sub(*B, *A);
	vec_t AC = vec_sub(*C, *A);
	
	// Normal of the triangle
	vec_t norm = vec_cross(AB, AC);
	float imag = 1.0f / sqrt(vec_dot(norm, norm));
	norm = vec_scale(norm, imag);
	
	float i = vec_dot(*ray_dir, norm);
	
	// No hit, Ray is parallel to triangle.
	if (i == 0.0) return 0;
	
	// Wrong side of the Triangle
	//if (i > 0.0) return 0; 
	
	// scalar for when ray intersects the plane that the triangle lies on.
	float t = vec_dot(vec_sub(*A, *ray_ori), norm) / i;
	
	if (t < 0.0f || t > rtn->dist) {
		return 0;
	}
	
	// intersection point of Ray and plane of the triangle
	vec_t X = vec_add(*ray_ori, vec_scale(*ray_dir, t));
	vec_t dX = vec_sub(X, *A);
	
	float d00 = vec_dot(AB, AB);
	float d01 = vec_dot(AB, AC);
	float d02 = vec_dot(AB, dX);
	float d11 = vec_dot(AC, AC);
	float d12 = vec_dot(AC, dX);
	
	float inv_denom = 1.0f / ((d01 * d01) - (d00 * d11));
	
	// Scalars:
	// X = A + (AB * u) + (AC * v)
	float u = ((d12 * d01) - (d02 * d11)) * inv_denom;
	float v = ((d02 * d01) - (d12 * d00)) * inv_denom;
	
	// Intersection point is outside triangle if:
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


int cast_ray_ellipsoid(vec_t * ray_ori, vec_t * ray_dir, ellipsoid_t * ellipsoid, vec_t * vec_list, intersect_t * rtn) {
	/*
	
	Ray: RayOrigin, RayRayDir->zection
		Ro, Rd
	
	Ellipseoid: origin, radius
		Eo, Er
	
	I = Ro + Rd * t
	
	( (I - So) / Sr ) dot ((I - So) / Sr) == 1
	
	V0 = Ro - Eo
	
	Quadratic Equation from Mathematica solver
	
	t = (-B +- Sqrt(B^2 - 4 *A*C)) / 2*A
	
	A = (Rd / Er) . (Rd / Er)
	B = 2.0f * (Rd / Er) . (V0 / Er)
	C = (V0 / Er) . (V0 / Er) - 1
	
	
	*/
	
	
	vec_t Er = *(vec_list + ellipsoid->radius);
	vec_t Rd = *ray_dir;
	vec_t v0 = vec_sub(*ray_ori, *(vec_list + ellipsoid->origin));
	
	float C = (((v0.x * v0.x) / (Er.x * Er.x)) + ((v0.y * v0.y) / (Er.y * Er.y)) + ((v0.z * v0.z) / (Er.z * Er.z))) - 1.0f;
	float B = 2.0f * (((Rd.x * v0.x) / (Er.x * Er.x)) + ((Rd.y * v0.y) / (Er.y * Er.y)) + ((Rd.z * v0.z) / (Er.z * Er.z)));
	float A = (((Rd.x * Rd.x) / (Er.x * Er.x)) + ((Rd.y * Rd.y) / (Er.y * Er.y)) + ((Rd.z * Rd.z) / (Er.z * Er.z)));
	
	if ((B * B) > (4.0f * A * C)) {
		
		float t1 = (-B + sqrt((B * B) - (4.0f * A * C))) / (2.0f * A);
		float t2 = (-B - sqrt((B * B) - (4.0f * A * C))) / (2.0f * A);
		
		float t = (t1 > 0.0f ? (t2 > 0.0f ? (t1 < t2 ? t1 : t2) : t1) : (t2 > 0.0f ? t2 : 0.0f));
		
		if (rtn->dist > t) {
			
			vec_t X = vec_add(*ray_ori, vec_scale(*ray_dir, t));
			vec_t norm = vec_sub(X, *(vec_list + ellipsoid->origin));
			norm = vec_scale(norm, 1.0f / vec_dot(norm, norm));
			
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

int cast_ray_cuboid(vec_t * RayOri, vec_t * RayDir, cuboid_t * cuboid, vec_t * vec_list, intersect_t * rtn) {
	
	return 0;
}


int cast_ray_primitive(vec_t * RayOri, vec_t * RayDir, primitive_t * primitive, vec_t * vec_list, intersect_t * rtn) {
	int A = 0;
	switch (primitive->type) {
		case TRIANGLE:
			A = cast_ray_triangle(RayOri, RayDir, (triangle_t *)(&primitive->data.triangle), vec_list, rtn);
			if (A != 0) {
				rtn->primitive = primitive;
			}
			return A;

		case ELLIPSOID:
			A = cast_ray_ellipsoid(RayOri, RayDir, (ellipsoid_t *)(&primitive->data.ellipsoid), vec_list, rtn);
			if (A != 0) {
				rtn->primitive = primitive;
			}
			return A;

		case CUBOID:
			A = cast_ray_cuboid(RayOri, RayDir, (cuboid_t *)(&primitive->data.cuboid), vec_list, rtn);
			if (A != 0) {
				rtn->primitive = primitive;
			}
			return A;

		default:
			return 0;

	}
}

int cast_ray_bvh(vec_t * RayOri, vec_t * RayDir, bvh_t * bvh, vec_t * vec_list, intersect_t * rtn) {
	
	if (bvh == NULL) return 0;
	
	float tx1 = (bvh->min.x - RayOri->x) / RayDir->x;
	float tx2 = (bvh->max.x - RayOri->x) / RayDir->x;
	float tx3 = (bvh->mid.x - RayOri->x) / RayDir->x;
	
	float ty1 = (bvh->min.y - RayOri->y) / RayDir->y;
	float ty2 = (bvh->max.y - RayOri->y) / RayDir->y;
	float ty3 = (bvh->mid.y - RayOri->y) / RayDir->y;
	
	float tz1 = (bvh->min.z - RayOri->z) / RayDir->z;
	float tz2 = (bvh->max.z - RayOri->z) / RayDir->z;
	float tz3 = (bvh->mid.z - RayOri->z) / RayDir->z;
	
	if (tx1 > tx2) {
		float tmp = tx1;
		tx1 = tx2;
		tx2 = tmp;
	}
	if (ty1 > ty2) {
		float tmp = ty1;
		ty1 = ty2;
		ty2 = tmp;
	}
	if (tz1 > tz2) {
		float tmp = tz1;
		tz1 = tz2;
		tz2 = tmp;
	}
	
	float tmp_min = tx1;
	if (ty1 > tmp_min) tmp_min = ty1;
	if (tz1 > tmp_min) tmp_min = tz1;
	
	float tmp_max = tx2;
	if (ty2 < tmp_max) tmp_max = ty2;
	if (tz2 < tmp_max) tmp_max = tz2;
	
	
	if (tmp_max < 0.0f || tmp_max < tmp_min) return 0;
	
	// The following code will determine the order of children of the bvh to check, and check them.
	// If you have questions on how this works, email me: duncmccl@udel.edu
	// I will provide an unobfuscated version
	
	if (bvh->children_count > 0) {
		
		unsigned char index = 
			(((RayDir->z > 0.0f) ^ (tz3 > tmp_min))<<2) | 
			(((RayDir->y > 0.0f) ^ (ty3 > tmp_min))<<1) | 
			(((RayDir->x > 0.0f) ^ (tx3 > tmp_min)));
		
		int A = cast_ray_bvh(RayOri, RayDir, bvh->children[index], vec_list, rtn);
		if (A) return A;
		
		float P[] = {
			1.0f / (tx3 - tmp_min),
			1.0f / (ty3 - tmp_min),
			1.0f / (tz3 - tmp_min)
		};
		unsigned char order = 
			((P[0] >= P[1]) << 4) | 
			((P[0] >= P[2]) << 3) | 
			((P[1] >= P[2]) << 2);
		const unsigned char search[32] = {
			2, 1, 0, 255,
			1, 2, 0, 255,
			255,255,255,255,
			1, 0, 2, 255,
			2, 0, 1, 255,
			255,255,255, 255,
			0, 2, 1, 255,
			0, 1, 2, 255
		};
		while(search[order] != 255) {
			if (P[search[order]] <= 0) return 0;
			index ^= 1 << search[order];
			A = cast_ray_bvh(RayOri, RayDir, bvh->children[index], vec_list, rtn);
			if (A) return A;
			order++;
		}
		
		return 0;

	} else {
		// Calls to check triangle contents
		int A = 0;
		for(int i = 0; i < bvh->primitive_count; i++) {
			int B = cast_ray_primitive(RayOri, RayDir, bvh->primitive_list + i, vec_list, rtn);
			if (B) A = B;
		}
		return A;
	}
	
}



// TODO: Replace primative list with a BVH
void trace_ray(vec_t * RayOri, vec_t * RayRayDir, bvh_t * bvh, vec_t * vec_list, color_t * rtn) {
	
	
	intersect_t I;
	I.dist = INFINITY;
	
	cast_ray_bvh(RayOri, RayRayDir, bvh, vec_list, &I);
	
	
	if (I.dist < INFINITY) {
		
		// This is where recursive calls would go, for reflections, refractions, lighting etc.
		
		
		float s = vec_dot(I.normal, *RayRayDir);
		s = abs(s);
		
		float r = (1.0f - I.u) * (1.0f - I.v);
		float g = (I.u) * (1.0f - I.v);
		float b = (1.0f - I.u) * (I.v);
		
		
		*rtn = (color_t){1.0f, s * r, s * g, s * b};
		
	} else {
		*rtn = (color_t){0.0f, 0.5f, 0.5f, 0.5f};
	}
	
}



unsigned int color_to_int(color_t color) {
	
	// A B G R
	
	return (((unsigned int)floor(color.a * 255.0f) & 0xFF) << 24) | 
		   (((unsigned int)floor(color.b * 255.0f) & 0xFF) << 16) | 
		   (((unsigned int)floor(color.g * 255.0f) & 0xFF) << 8) | 
		   (((unsigned int)floor(color.r * 255.0f) & 0xFF) << 0);
	
}

void render_image(camera_t * Camera, model_t * render_target, unsigned int * PixelBuffer) {
	
	bvh_t * bvh = build_bvh(render_target, 16, 8);
	
	
	#pragma omp parallel for
	for(int i = 0; i < Camera->vRES * Camera->hRES; i++) {
		
		int x = i % Camera->hRES;
		int y = i / Camera->hRES;
		
		float Pitch = (Camera->vFOV / (float)Camera->vRES) * y - (Camera->vFOV / 2.0f);
		float Yaw = (Camera->hFOV / 2.0f) - (Camera->hFOV / (float)Camera->hRES) * x;

		vec_t axis = vec_add(vec_scale(Camera->up, Yaw), vec_scale(Camera->right, Pitch));
		float im = 1.0f / sqrt(vec_dot(axis, axis));
		axis = vec_scale(axis, im);

		vec_t RayDir = vec_rotate(Camera->look, axis, sqrt(Yaw*Yaw + Pitch*Pitch));

		color_t color = (color_t){0.0, 0.0, 0.0, 0.0};
		trace_ray(&Camera->pos, &RayDir, bvh, render_target->vec_list, &color);

		PixelBuffer[x + y * Camera->hRES] = color_to_int(color);

	}
	
	destroy_bvh(bvh);
	
}
