#include <math.h>
#include "vec3.h"

struct vec3 vec3_add(const struct vec3 *v1, const struct vec3 *v2)
{
	return (struct vec3) {v1->x + v2->x, v1->y + v2->y, v1->z + v2->z};
}


struct vec3 vec3_sub(const struct vec3 *v1, const struct vec3 *v2)
{
	return (struct vec3) {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
}


float vec3_dotp(const struct vec3 *v1, const struct vec3 *v2)
{
	return ((v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z));
}


struct vec3 vec3_crossp(const struct vec3 *v1, const struct vec3 *v2)
{
	return (struct vec3) {
		(v1->y * v2->z) - (v1->z * v2->y),
		(v1->z * v2->x) - (v1->x * v2->z),
		(v1->x * v2->y) - (v1->y * v2->x)
	};
}


struct vec3 vec3_scalarmult(float c, const struct vec3 *v)
{
	return (struct vec3) {c * v->x, c * v->y, c * v->z};
}


float vec3_norm(const struct vec3 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

}


struct vec3 vec3_unitv(const struct vec3 *v)
{
	float OneOverNorm = 1 / vec3_norm(v);
	return vec3_scalarmult(OneOverNorm, v);
}


float vec3_anglebetween(const struct vec3 *v1, const struct vec3 *v2)
{
	float dotp = vec3_dotp(v1, v2);
	float norm_v1 = vec3_norm(v1);
	float norm_v2 = vec3_norm(v2);

	return acosf(dotp / (norm_v1 * norm_v2));
}
