#include "vec3.h"

#include <stdio.h>

void pvec3(struct vec3 v)
{
        printf("v = (%f,%f,%f)\n",v.x, v.y, v.z);
}


int main(int argc, char **argv)
{

        struct vec3 v1 = {3.0, -4.0, 5.0};

        struct vec3 v2 = {0.0, 5.5, 9.4};

        pvec3(vec3_add(&v1,&v2));

        pvec3(vec3_sub(&v1,&v2));

        printf("%f\n", vec3_dotp(&v1,&v2));

        pvec3(vec3_crossp(&v1,&v2));

        pvec3(vec3_scalarmult(3,&v2));

        printf("%f\n", vec3_norm(&v1));

        pvec3(vec3_unitv(&v1));

        printf("%f\n", vec3_anglebetween(&v1, &v2));

        return 0;
}
