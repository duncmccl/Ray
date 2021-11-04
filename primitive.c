#include "primitive.h"


float vec_dot(const vec_t A, const vec_t B) {
	return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}

vec_t vec_cross(const vec_t A, const vec_t B) {
	return (vec_t){((A.y * B.z) - (A.z * B.y)), 
				   ((A.z * B.x) - (A.x * B.z)), 
				   ((A.x * B.y) - (A.y * B.x))};
}

vec_t vec_scale(const vec_t A, float s) {
	return (vec_t){A.x * s, A.y * s, A.z * s};
}

vec_t vec_add(const vec_t A, const vec_t B) {
	return (vec_t){A.x + B.x, A.y + B.y, A.z + B.z};
}

vec_t vec_sub(const vec_t A, const vec_t B) {
	return (vec_t){A.x - B.x, A.y - B.y, A.z - B.z};
}

vec_t vec_rotate(const vec_t A, const vec_t B, float t) {
	// Ru(t)x = u * DOT(u, x) + cos(t) * CROSS(CROSS(u, x), u) + sin(t) * CROSS(u, x)
	
	vec_t w = vec_cross(B, A);
	vec_t v = vec_cross(w, B);
	
	vec_t para = vec_scale(B, vec_dot(A, B));
	
	vec_t vsin = vec_scale(w, sin(t));
	
	vec_t vcos = vec_scale(v, cos(t));
	
	return vec_add(para, vec_add(vsin, vcos));
	
}