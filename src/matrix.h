#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct
{
    float m[4][4];
} mat4_t;

mat4_t mat4_identity();
mat4_t mat4_translate(float x, float y, float z);
mat4_t mat4_scale(float x, float y, float z);
mat4_t mat4_rotate(float angle, float x, float y, float z);
mat4_t mat4_mul(mat4_t a, mat4_t b);
mat4_t mat4_mul_vec4(mat4_t m, vec4_t v);

#endif