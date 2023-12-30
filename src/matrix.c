#include "matrix.h"
#include <math.h>

mat4_t mat4_identity()
{
    mat4_t m = {0};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
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

mat4_t mat4_make_rotation_x(float angle)
{
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_y(float angle)
{
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_z(float angle)
{
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

mat4_t mat4_mul(mat4_t a, mat4_t b)
{
    mat4_t r = {0};
    for (int i = 0; i < 4; i++)
    {
        r.m[i][0] = a.m[i][0] * b.m[0][0] + a.m[i][1] * b.m[1][0] + a.m[i][2] * b.m[2][0] + a.m[i][3] * b.m[3][0];
        r.m[i][1] = a.m[i][0] * b.m[0][1] + a.m[i][1] * b.m[1][1] + a.m[i][2] * b.m[2][1] + a.m[i][3] * b.m[3][1];
        r.m[i][2] = a.m[i][0] * b.m[0][2] + a.m[i][1] * b.m[1][2] + a.m[i][2] * b.m[2][2] + a.m[i][3] * b.m[3][2];
        r.m[i][3] = a.m[i][0] * b.m[0][3] + a.m[i][1] * b.m[1][3] + a.m[i][2] * b.m[2][3] + a.m[i][3] * b.m[3][3];
    }
    return r;
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

mat4_t mat4_make_perspective(float fov, float aspect, float near, float far)
{
    mat4_t m = {0};
    // float fov_rad = 1.0f / tan(fov * 0.5f / 180.0f * 3.14159265358979323846f);
    float fovtain = 1.0f / tan(fov * 0.5f);
    m.m[0][0] = aspect * fovtain;
    m.m[1][1] = fovtain;
    m.m[2][2] = far / (far - near);
    m.m[3][2] = (-far * near) / (far - near);
    m.m[2][3] = 1.0f;
    return m;
}

vec4_t mat4_mul_vec4_project(mat4_t m, vec4_t v)
{
    vec4_t r = mat4_mul_vec4(m, v);
    if (r.w != 0.0f)
    {
        r.x /= r.w;
        r.y /= r.w;
        r.z /= r.w;
    }
    return r;
}