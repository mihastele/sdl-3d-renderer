#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;
int previous_frame_time = 0;

float fov_factor = 640;

// #define N_POINTS (9 * 9 * 9)
// vec3_t cube_points[N_POINTS];
// vec2_t projected_points[N_POINTS];

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

void setup(void)
{
    /* Add setup code here */
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

    // int point_count = 0;

    // for (float x = -1; x <= 1; x += 0.25)
    // {
    //     for (float y = -1; y <= 1; y += 0.25)
    //     {
    //         for (float z = -1; z <= 1; z += 0.25)
    //         {
    //             vec3_t new_point = {.x = x, .y = y, .z = z};
    //             cube_points[point_count++] = new_point;
    //         }
    //     }
    // }
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}

// Returns projected 2D point
vec2_t project(vec3_t point)
{
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z};

    return projected_point;
}

void update(void)
{

    // waste/delay until we reach the target frame time

    // Don't use while loop, it will use 100% CPU for that task, don't waste CPU resources to be locked doing nothing
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME))
    // {
    // }

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    previous_frame_time = SDL_GetTicks();

    cube_rotation.y += 0.01;
    cube_rotation.x += 0.04;
    cube_rotation.z += 0.02;

    /* Add update code here */
    // for (int i = 0; i < N_POINTS; i++)
    // {
    //     vec3_t point = cube_points[i];

    //     vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
    //     transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
    //     transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

    //     transformed_point.z -= camera_position.z;

    //     vec2_t projected_point = project(transformed_point);
    //     // int x = (projected_point.x + 1.0) * (window_width / 2.0);
    //     // int y = (projected_point.y + 1.0) * (window_height / 2.0);

    //     // draw_rect(x, y, 4, 4, 0xFFFFFFFF);
    //     projected_points[i] = projected_point;
    // }

    for (int i = 0; i < N_MESH_FACES; i++)
    {
        face_t mesh_face = mesh_faces[i];

        vec3_t face_vertices[3] = {
            mesh_vertices[mesh_face.a - 1],
            mesh_vertices[mesh_face.b - 1],
            mesh_vertices[mesh_face.c - 1],
        };

        triangle_t projected_triangle;

        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

            transformed_vertex.z -= camera_position.z;

            face_vertices[j] = transformed_vertex;

            vec2_t projected_point = project(transformed_vertex);

            // scale and project to the middle
            projected_point.x += window_width / 2;
            projected_point.y += window_height / 2;

            projected_triangle.points[j] = projected_point;
        }

        triangles_to_render[i] = projected_triangle;
    }
}

void render(void)
{
    // we are using color buffer now
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderClear(renderer);

    // for (int i = 0; i < N_POINTS; i++)
    // {
    //     vec2_t projected_point = projected_points[i];
    //     // int x = (projected_points[i].x + 1.0) * (window_width / 2.0);
    //     // int y = (projected_points[i].y + 1.0) * (window_height / 2.0);

    //     draw_rect(
    //         projected_point.x + window_width / 2,
    //         projected_point.y + window_height / 2,
    //         4,
    //         4,
    //         0xFFFFFF00);
    // }

    for (int i = 0; i < N_MESH_FACES; i++)
    {

        triangle_t triangle = triangles_to_render[i];

        draw_rect(
            triangle.points[0].x,
            triangle.points[0].y,
            4,
            4,
            0xFFFFFFFF);

        draw_rect(
            triangle.points[1].x,
            triangle.points[1].y,
            4,
            4,
            0xFFFFFFFF);

        draw_rect(
            triangle.points[2].x,
            triangle.points[2].y,
            4,
            4,
            0xFFFFFFFF);
    }

    draw_grid(55, 0xFFFFFFFF);

    // draw_pixel(406, 406, 0xFFFF0000);

    // draw_rect(0, 0, 200, 200, 0xFFFF0000);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    /* Create SDL window */
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}