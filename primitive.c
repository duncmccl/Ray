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
	
	float dAB = ((A.x * B.x) + (A.y * B.y) + (A.z * B.z));
	float St = sin(t);
	float Ct = cos(t);
	
	return (vec_t){
		((B.x * dAB) + (w.x * St) + (v.x * Ct)), 
		((B.y * dAB) + (w.y * St) + (v.y * Ct)), 
		((B.z * dAB) + (w.z * St) + (v.z * Ct))
	};
	
}