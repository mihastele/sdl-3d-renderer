#include <stdio.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {.x = 0, .y = 0, .z = 0},
};

vec3_t mesh_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1},
    {.x = -1, .y = 1, .z = -1},
    {.x = 1, .y = 1, .z = -1},
    {.x = 1, .y = -1, .z = -1},
    {.x = 1, .y = 1, .z = 1},
    {.x = 1, .y = -1, .z = 1},
    {.x = -1, .y = 1, .z = 1},
    {.x = -1, .y = -1, .z = 1},
};

face_t mesh_faces[N_CUBE_FACES] = {
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        vec3_t vertex = mesh_vertices[i];
        array_push(mesh.vertices, vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; i++)
    {
        face_t face = mesh_faces[i];
        array_push(mesh.faces, face);
    }
}

void load_obj_file_data(char *filename)
{
    FILE *fp = fopen(filename, "r");

    char line[1024]; // max lenght 1024 for simpler impementation

    while (fgets(line, 1024, fp))
    {
        printf("LINE=%s", line);
    }
}