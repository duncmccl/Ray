#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "primitive.h"
#include "btree.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


float vec_dot(vec_t A, vec_t B) {
	return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}

vec_t vec_cross(vec_t A, vec_t B) {
	return (vec_t){((A.y * B.z) - (A.z * B.y)), 
				   ((A.z * B.x) - (A.x * B.z)), 
				   ((A.x * B.y) - (A.y * B.x))};
}

vec_t vec_scale(vec_t A, float s) {
	return (vec_t){A.x * s, A.y * s, A.z * s};
}

vec_t vec_add(vec_t A, vec_t B) {
	return (vec_t){A.x + B.x, A.y + B.y, A.z + B.z};
}

vec_t vec_sub(vec_t A, vec_t B) {
	return (vec_t){A.x - B.x, A.y - B.y, A.z - B.z};
}

vec_t vec_rotate(vec_t A, vec_t B, float t) {
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



model_t * load_model(const char * fname) {
	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fname, NULL, true)) {
		throw std::runtime_error(warn + err);
	}
	
	int vec_count = attrib.vertices.size() / 3;
	vec_t * vec_list = (vec_t *) malloc(sizeof(vec_t) * vec_count);
	for(int i = 0; i < vec_count; i++) {
		vec_list[i] = (vec_t){attrib.vertices[i*3 + 0], attrib.vertices[i*3 + 1], attrib.vertices[i*3 + 2]};
	}
	
	
	int primitive_count = 0;
	for(unsigned long i = 0; i < shapes.size(); i++) {
		primitive_count += shapes[i].mesh.indices.size() / 3;
	}
	
	primitive_t * primitive_list = (primitive_t *) malloc(sizeof(primitive_t) * primitive_count);
	int primitive_index = 0;
	for(unsigned long i = 0; i < shapes.size(); i++) {
		for(unsigned long j = 0; j < shapes[i].mesh.indices.size(); j+= 3) {
			tinyobj::index_t iA = shapes[i].mesh.indices[j];
			tinyobj::index_t iB = shapes[i].mesh.indices[j+1];
			tinyobj::index_t iC = shapes[i].mesh.indices[j+2];
			
			primitive_list[primitive_index].type = TRIANGLE;
			primitive_list[primitive_index].data.triangle = (triangle_t){vec_list + iA.vertex_index, vec_list + iB.vertex_index, vec_list + iC.vertex_index};
			
			primitive_index++;
		}
	}
	
	model_t * rtn = (model_t *) malloc(sizeof(model_t));
	
	rtn->vec_count = vec_count;
	rtn->vec_list = vec_list;
	
	rtn->primitive_count = primitive_count;
	rtn->primitive_list = primitive_list;
	
	return rtn;
	
}
model_t * aggregate_models(model_t ** model_list, int model_count) {
	
	model_t * rtn = (model_t *) malloc(sizeof(model_t));
	
	int vec_total = 0;
	int primitive_total = 0;
	for(int i = 0; i < model_count; i++) {
		vec_total += model_list[i]->vec_count;
		primitive_total += model_list[i]->primitive_count;
	}
	
	rtn->vec_count = vec_total;
	rtn->vec_list = (vec_t *) malloc(sizeof(vec_t) * rtn->vec_count);
	
	rtn->primitive_count = primitive_total;
	rtn->primitive_list = (primitive_t *) malloc(sizeof(primitive_t) * primitive_total);
	
	int vec_offset = 0;
	int primitive_offset = 0;
	for(int i = 0; i < model_count; i++) {
		memcpy(rtn->vec_list + vec_offset, model_list[i]->vec_list, sizeof(vec_t) * model_list[i]->vec_count);
		vec_offset += model_list[i]->vec_count;
		
		memcpy(rtn->primitive_list + primitive_offset, model_list[i]->primitive_list, sizeof(primitive_t) * model_list[i]->primitive_count);
		primitive_offset += model_list[i]->primitive_count;
	}
	
	return rtn;
	
}

model_t * copy_model(model_t * target) {
	model_t * rtn = (model_t *) malloc(sizeof(model_t));
	rtn->vec_count = target->vec_count;
	rtn->vec_list = (vec_t *) malloc(sizeof(vec_t) * rtn->vec_count);
	memcpy(rtn->vec_list, target->vec_list, sizeof(vec_t) * rtn->vec_count);
	
	rtn->primitive_count = target->primitive_count;
	rtn->primitive_list = (primitive_t *) malloc(sizeof(primitive_t) * rtn->primitive_count);
	memcpy(rtn->primitive_list, target->primitive_list, sizeof(primitive_t) * rtn->primitive_count);
	
	
	for(int i = 0; i < rtn->primitive_count; i++) {
		
		switch (rtn->primitive_list[i].type) {
			case TRIANGLE:
				
				rtn->primitive_list[i].data.triangle.A = rtn->vec_list + (rtn->primitive_list[i].data.triangle.A - target->vec_list);
				rtn->primitive_list[i].data.triangle.B = rtn->vec_list + (rtn->primitive_list[i].data.triangle.B - target->vec_list);
				rtn->primitive_list[i].data.triangle.C = rtn->vec_list + (rtn->primitive_list[i].data.triangle.C - target->vec_list);
				
				break;
				
			case SPHERE:
				
				rtn->primitive_list[i].data.sphere.ori = rtn->vec_list + (rtn->primitive_list[i].data.sphere.ori - target->vec_list);
				
				break;
				
			case CUBOID:
				
				rtn->primitive_list[i].data.cuboid.min = rtn->vec_list + (rtn->primitive_list[i].data.cuboid.min - target->vec_list);
				rtn->primitive_list[i].data.cuboid.max = rtn->vec_list + (rtn->primitive_list[i].data.cuboid.max - target->vec_list);
				
				break;
				
			default:
				break;
		}
		
	}
	
	
	
	return rtn;
}

void trans_rotate_model(model_t * target, vec_t * look, vec_t * up, vec_t * right, vec_t * pos) {
	
	for(int i = 0; i < target->vec_count; i++) {
		
		vec_t * old = target->vec_list +i;
		
		vec_t new_vec = (vec_t){
			(old->x * look->x) + (old->y * up->x) + (old->z * right->x) + pos->x,
			(old->x * look->y) + (old->y * up->y) + (old->z * right->y) + pos->y,
			(old->x * look->z) + (old->y * up->z) + (old->z * right->z) + pos->z
		};
		
		*old = new_vec;
	}
}

void destroy_model(model_t * model) {
	if (model) {
		free(model->vec_list);
		free(model->primitive_list);
		free(model);
	}
}



void get_bounds_triangle(triangle_t * triangle, vec_t * p_min, vec_t * p_mid, vec_t * p_max) {
	
	vec_t *A = triangle->A;
	vec_t *B = triangle->B;
	vec_t *C = triangle->C;
	
	if (A->x < B->x && A->x < C->x) {
		p_min->x = A->x;
	} else if (B->x < C->x) {
		p_min->x = B->x;
	} else {
		p_min->x = C->x;
	}
	
	if (A->y < B->y && A->y < C->y) {
		p_min->y = A->y;
	} else if (B->y < C->y) {
		p_min->y = B->y;
	} else {
		p_min->y = C->y;
	}
	
	if (A->z < B->z && A->z < C->z) {
		p_min->z = A->z;
	} else if (B->z < C->z) {
		p_min->z = B->z;
	} else {
		p_min->z = C->z;
	}
	
	
	
	if (A->x > B->x && A->x > C->x) {
		p_max->x = A->x;
	} else if (B->x > C->x) {
		p_max->x = B->x;
	} else {
		p_max->x = C->x;
	}
	
	if (A->y > B->y && A->y > C->y) {
		p_max->y = A->y;
	} else if (B->y > C->y) {
		p_max->y = B->y;
	} else {
		p_max->y = C->y;
	}
	
	if (A->z > B->z && A->z > C->z) {
		p_max->z = A->z;
	} else if (B->z > C->z) {
		p_max->z = B->z;
	} else {
		p_max->z = C->z;
	}
	
	
	
	p_mid->x = (A->x + B->x + C->x) / 3.0f;
	p_mid->y = (A->y + B->y + C->y) / 3.0f;
	p_mid->z = (A->z + B->z + C->z) / 3.0f;
	
}

void get_bounds_sphere(sphere_t * sphere, vec_t * p_min, vec_t * p_mid, vec_t * p_max) {
	*p_min = (vec_t){sphere->ori->x - *sphere->radius, sphere->ori->y - *sphere->radius, sphere->ori->z - *sphere->radius};
	*p_mid = *sphere->ori;
	*p_max = (vec_t){sphere->ori->x + *sphere->radius, sphere->ori->y + *sphere->radius, sphere->ori->z + *sphere->radius};
}

void get_bounds_cuboid(cuboid_t * cuboid, vec_t * p_min, vec_t * p_mid, vec_t * p_max) {
	*p_min = *cuboid->min;
	*p_mid = (vec_t){(cuboid->min->x + cuboid->max->x) * 0.5f, (cuboid->min->y + cuboid->max->y) * 0.5f, (cuboid->min->z + cuboid->max->z) * 0.5f};
	*p_max = *cuboid->max;
}

void get_bounds_primitive(primitive_t * primitive, vec_t * p_min, vec_t * p_mid, vec_t * p_max) {
	
	switch (primitive->type) {
		case TRIANGLE:
			get_bounds_triangle(&primitive->data.triangle, p_min, p_mid, p_max);
			break;
			
		case SPHERE:
			get_bounds_sphere(&primitive->data.sphere, p_min, p_mid, p_max);
			break;
			
		case CUBOID:
			get_bounds_cuboid(&primitive->data.cuboid, p_min, p_mid, p_max);
			break;
			
		default:
			break;
	}
}

int overlap_bounds(vec_t * A_min, vec_t * A_max, vec_t * B_min, vec_t * B_max) {
	
	return (A_min->x <= B_max->x && A_max->x >= B_min->x) && 
		(A_min->y <= B_max->y && A_max->y >= B_min->y) && 
		(A_min->z <= B_max->z && A_max->z >= B_min->z);
	
}

unsigned long hash_vec(vec_t * vec) {
	return (((unsigned long)floor(vec->x * 607.0)) & 0x3FF) | ((((unsigned long)floor(vec->y * 47.0)) & 0x3FF) << 10) | ((((unsigned long)floor(vec->z * 809.0)) & 0x3FF) << 20);
}

bvh_t * create_bvh(int primitive_count, primitive_t * primitive_list, vec_t * smin, vec_t * smax, int limit, int depth) {
	
	if (depth < 0) return NULL;
	if (primitive_count <= 0) return NULL;
	
	bvh_t * rtn = (bvh_t *) malloc(sizeof(bvh_t));
	
	unsigned char bSetMinMax = (smin == NULL || smax == NULL);
	
	if(bSetMinMax) {
		rtn->min = (vec_t){INFINITY, INFINITY, INFINITY};
		rtn->max = (vec_t){-INFINITY, -INFINITY, -INFINITY};
	} else {
		rtn->min = *smin;
		rtn->max = *smax;
	}
	
	rtn->mid = (vec_t){0.0, 0.0, 0.0};
	int vec_count = 0;
	btree_t * vec_tree = NULL;
	
	for(int i = 0; i < primitive_count; i++) {
		
		vec_t p_min, p_mid, p_max;
		get_bounds_primitive(primitive_list + i, &p_min, &p_mid, &p_max);
		
		
		unsigned long h = hash_vec(&p_mid);
		if (!contains_btree(vec_tree, h)) {
			insert_btree(&vec_tree, h);

			rtn->mid.x += p_mid.x;
			rtn->mid.y += p_mid.y;
			rtn->mid.z += p_mid.z;
			vec_count++;
		}
		
		if (bSetMinMax) {
			if(rtn->min.x > p_min.x) rtn->min.x = p_min.x;
			if(rtn->min.y > p_min.y) rtn->min.y = p_min.y;
			if(rtn->min.z > p_min.z) rtn->min.z = p_min.z;

			if(rtn->max.x < p_max.x) rtn->max.x = p_max.x;
			if(rtn->max.y < p_max.y) rtn->max.y = p_max.y;
			if(rtn->max.z < p_max.z) rtn->max.z = p_max.z;
		}
		
		
	}
	
	free_btree(vec_tree);
	
	if (vec_count != 0) {
		rtn->mid.x /= (float)vec_count;
		rtn->mid.y /= (float)vec_count;
		rtn->mid.z /= (float)vec_count;
	}
	
	if((rtn->mid.x < rtn->min.x) || (rtn->mid.x > rtn->max.x) || (rtn->mid.y < rtn->min.y) || (rtn->mid.y > rtn->max.y) || (rtn->mid.z < rtn->min.z) || (rtn->mid.z > rtn->max.z)) {
		
		rtn->mid.x = (rtn->min.x + rtn->max.x) * 0.5f;
		rtn->mid.y = (rtn->min.y + rtn->max.y) * 0.5f;
		rtn->mid.z = (rtn->min.z + rtn->max.z) * 0.5f;
		
	}
	
	if (primitive_count > limit && depth > 0) {
		
		for(int i = 0; i < 8; i++) {
			
			vec_t tmp_min = (vec_t){(i & 0x1) ? rtn->mid.x : rtn->min.x, 
									 (i & 0x2) ? rtn->mid.y : rtn->min.y, 
									 (i & 0x4) ? rtn->mid.z : rtn->min.z};
			vec_t tmp_max = (vec_t){(i & 0x1) ? rtn->max.x : rtn->mid.x, 
									 (i & 0x2) ? rtn->max.y : rtn->mid.y, 
									 (i & 0x4) ? rtn->max.z : rtn->mid.z};
			
			primitive_t * temp_primitive_list = (primitive_t *) malloc(sizeof(primitive_t) * primitive_count);
			
			for(int j = 0; j < primitive_count; j++) {
				
				vec_t p_min, p_mid, p_max;
				get_bounds_primitive(primitive_list + j, &p_min, &p_mid, &p_max);
				
				if (overlap_bounds(&tmp_min, &tmp_max, &p_min, &p_max)) {
					temp_primitive_list[j] = primitive_list[j];
				} else {
					temp_primitive_list[j].type = INVALID;
				}
				
			}
			
			int count = 0;
			for(int j = 0; j < primitive_count; j++) {
				if (temp_primitive_list[j].type != INVALID) {
					temp_primitive_list[count] = temp_primitive_list[j];
					count++;
				}
			}
			
			rtn->children[i] = create_bvh(count, temp_primitive_list, &tmp_min, &tmp_max, limit, depth-1);
			free(temp_primitive_list);
			
		}
		
		
		int children_count = 0;
		for(int i = 0; i < 8; i++) {
			if (rtn->children[i] != NULL) children_count++;
		}
		if (children_count != 0) {
			
			rtn->children_count = children_count;
			rtn->primitive_list = NULL;
			rtn->primitive_count = 0;
			return rtn;
			
		} else {
			
			free(rtn);
			return NULL;
		}
		
	} else {
		
		rtn->children_count = 0;
		for(int i = 0; i < 8; i++) rtn->children[i] = NULL;
		
		rtn->primitive_list = (primitive_t *) malloc(sizeof(primitive_t) * primitive_count);
		rtn->primitive_count = primitive_count;
		memcpy(rtn->primitive_list, primitive_list, sizeof(primitive_t) * primitive_count);
		
		return rtn;
	}
}

bvh_t * build_bvh(model_t * model, int limit, int depth) {
	return create_bvh(model->primitive_count, model->primitive_list, NULL, NULL, limit, depth);
}

void destroy_bvh(bvh_t * bvh) {
	if (bvh) {
		if (bvh->children_count > 0) {
			for(int i = 0; i < 8; i++) destroy_bvh(bvh->children[i]);
		}
		if (bvh->primitive_count > 0) free(bvh->primitive_list);
		free(bvh);
	}
}

