#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>

typedef struct
{
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

typedef struct
{
    vec2_t points[3];
    uint32_t color;
    float avg_depth;
} triangle_t;

#endif // TRIANGLE_H