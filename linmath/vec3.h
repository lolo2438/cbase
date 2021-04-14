#ifndef VEC3_H
#define VEC3_H

struct vec3 {
	float x,y,z;
};

struct vec3 vec3_add(const struct vec3 *v1, const struct vec3 *v2);

struct vec3 vec3_sub(const struct vec3 *v1, const struct vec3 *v2);

float vec3_dotp(const struct vec3 *v1, const struct vec3 *v2);

struct vec3 vec3_crossp(const struct vec3 *v1, const struct vec3 *v2);

struct vec3 vec3_scalarmult(float c, const struct vec3 *v);

float vec3_norm(const struct vec3 *v);

struct vec3 vec3_unitv(const struct vec3 *v);

float vec3_anglebetween(const struct vec3 *v1, const struct vec3 *v2);

#endif
