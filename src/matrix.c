#include "matrix.h"

mat4_t mat4_identity()
{
    mat4_t m = {0};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[3][0] = tx;
    m.m[3][1] = ty;
    m.m[3][2] = tz;
    return m;
}

mat4_t mat4_make_scale(float x, float y, float z)
{
    mat4_t m = mat4_identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
    vec4_t r = {0};
    r.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w;
    r.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w;
    r.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w;
    r.w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w;
    return r;
}

