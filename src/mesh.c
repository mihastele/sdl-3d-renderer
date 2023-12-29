#include <stdio.h>
#include <string.h>
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

    if (fp != NULL)
    {
        while (fgets(line, 1024, fp))
        {
            // printf("LINE=%s", line);
            if (strncmp(line, "v ", 2) == 0)
            {
                // printf("VERT\n");
                vec3_t vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                array_push(mesh.vertices, vertex);
            }
            else if (strncmp(line, "f ", 2) == 0)
            {
                face_t face;
                int texture_indices[3] = {-1, -1, -1};
                int normal_indices[3] = {-1, -1, -1};

                char *token = strtok(line, " "); // Skip the "f"

                for (int i = 0; i < 3; i++)
                {
                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        printf("Error: face line has less than 3 vertices\n");
                        return;
                    }

                    int *face_idx;

                    switch (i)
                    {
                    case 0:
                        face_idx = &face.a;
                        break;
                    case 1:
                        face_idx = &face.b;
                        break;
                    case 2:
                        face_idx = &face.c;
                        break;
                    default:
                        break;
                    }

                    // Try to parse the token in all possible formats
                    if (sscanf(token, "%d/%d/%d", face_idx, &texture_indices[i], &normal_indices[i]) != 3)
                    {
                        if (sscanf(token, "%d//%d", face_idx, &normal_indices[i]) != 2)
                        {
                            if (sscanf(token, "%d/%d", face_idx, &texture_indices[i]) != 2)
                            {
                                if (sscanf(token, "%d", face_idx) != 1)
                                {
                                    printf("Error: face line has invalid format\n");
                                    return;
                                }
                            }
                        }
                    }
                }

                array_push(mesh.faces, face);
            }
        }
    }
    fclose(fp);
}