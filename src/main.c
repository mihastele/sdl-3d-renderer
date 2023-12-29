#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

bool is_running = false;
int previous_frame_time = 0;

float fov_factor = 640;

// #define N_POINTS (9 * 9 * 9)
// vec3_t cube_points[N_POINTS];
// vec2_t projected_points[N_POINTS];

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {0, 0, 0};
// vec3_t mesh.rotation = {.x = 0, .y = 0, .z = 0};

enum Render_method render_method;
enum Cull_method cull_method;

void setup(void)
{

    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

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

    load_obj_file_data("./assets/f22.obj");
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
        {
            is_running = false;
        }
        if (event.key.keysym.sym == SDLK_1)
        {
            render_method = RENDER_WIRE_VERTEX;
        }
        if (event.key.keysym.sym == SDLK_2)
        {
            render_method = RENDER_WIRE;
        }
        if (event.key.keysym.sym == SDLK_3)
        {
            render_method = RENDER_FILL_TRIANGLE;
        }
        if (event.key.keysym.sym == SDLK_4)
        {
            render_method = RENDER_FILL_TRIANGLE_WIRE;
        }
        if (event.key.keysym.sym == SDLK_5)
        {
            render_method = RENDER_VERTEX;
        }
        if (event.key.keysym.sym == SDLK_c)
        {
            cull_method = CULL_BACKFACE;
        }
        if (event.key.keysym.sym == SDLK_d)
        {
            cull_method = CULL_NONE;
        }
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

void quicksort_rec(triangle_t *items, int left, int right)
{
    int i, j;
    triangle_t pivot, temp;

    i = left;
    j = right;

    pivot = items[(left + right) / 2];

    do
    {
        while ((items[i].avg_depth < pivot.avg_depth) && (i < right))
        {
            i++;
        }
        while ((pivot.avg_depth < items[j].avg_depth) && (j > left))
        {
            j--;
        }
        if (i <= j)
        {
            temp = items[i];
            items[i] = items[j];
            items[j] = temp;
            i++;
            j--;
        }
    } while (i <= j);

    if (left < j)
    {
        quicksort_rec(items, left, j);
    }
    if (i < right)
    {
        quicksort_rec(items, i, right);
    }
}

void quicksort(triangle_t *items)
{
    quicksort_rec(items, 0, array_length(items) - 1);
}

void update(void)
{

    // initialize Arrays to render
    triangles_to_render = NULL;

    // waste/delay until we reach the target frame time

    // Don't use while loop, it will use 100% CPU for that task, don't waste CPU resources to be locked doing nothing
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME))
    // {
    // }

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    previous_frame_time = SDL_GetTicks();

    mesh.rotation.y += 0.01;
    mesh.rotation.x += 0.03;
    mesh.rotation.z += 0.04;

    /* Add update code here */
    // for (int i = 0; i < N_POINTS; i++)
    // {
    //     vec3_t point = cube_points[i];

    //     vec3_t transformed_point = vec3_rotate_y(point, mesh.rotation.y);
    //     transformed_point = vec3_rotate_x(transformed_point, mesh.rotation.x);
    //     transformed_point = vec3_rotate_z(transformed_point, mesh.rotation.z);

    //     transformed_point.z -= camera_position.z;

    //     vec2_t projected_point = project(transformed_point);
    //     // int x = (projected_point.x + 1.0) * (window_width / 2.0);
    //     // int y = (projected_point.y + 1.0) * (window_height / 2.0);

    //     // draw_rect(x, y, 4, 4, 0xFFFFFFFF);
    //     projected_points[i] = projected_point;
    // }

    int num_faces = array_length(mesh.faces);

    for (int i = 0; i < num_faces; i++)
    {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3] = {
            mesh.vertices[mesh_face.a - 1],
            mesh.vertices[mesh_face.b - 1],
            mesh.vertices[mesh_face.c - 1],
        };

        vec3_t transformed_vertices[3];

        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // translate the vertex away from the camera
            transformed_vertex.z += 5; //  -= -5

            transformed_vertices[j] = transformed_vertex;
        }

        if (cull_method == CULL_BACKFACE)
        {
            // backface culling begins here
            vec3_t vector_a = transformed_vertices[0];
            vec3_t vector_b = transformed_vertices[1];
            vec3_t vector_c = transformed_vertices[2];

            vec3_t vector_ab = vec3_sub(vector_b, vector_a);
            vec3_t vector_ac = vec3_sub(vector_c, vector_a);
            vec3_normalize_inplace(&vector_ab);
            vec3_normalize_inplace(&vector_ac);

            vec3_t normal = vec3_cross(vector_ab, vector_ac);

            // normalize the normal
            vec3_normalize_inplace(&normal);

            vec3_t camera_ray = vec3_sub(camera_position, vector_a);

            float dot_product = vec3_dot(normal, camera_ray);

            // skip the triangle if it's facing away from the camera
            if (dot_product < 0)
            {
                continue;
            }
        }

        // triangle_t projected_triangle;

        vec2_t projected_points[3];

        for (int j = 0; j < 3; j++)
        {

            projected_points[j] = project(transformed_vertices[j]);

            // scale and project to the middle
            projected_points[j].x += window_width / 2;
            projected_points[j].y += window_height / 2;

            // projected_triangle.points[j] = projected_points[j];
        }

        // calculate average depth based on the vertice tranformation
        float avg_depth =
            (transformed_vertices[0].z +
             transformed_vertices[1].z +
             transformed_vertices[2].z) /
            3.0;

        triangle_t projected_triangle = {
            .points[0] = projected_points[0],
            .points[1] = projected_points[1],
            .points[2] = projected_points[2],
            .color = mesh_face.color,
            .avg_depth = avg_depth};

        // triangles_to_render[i] = projected_triangle;
        array_push(triangles_to_render, projected_triangle);
    }

    // TODO: sort triangles to render by avg_depth
    quicksort(triangles_to_render);

    // for (int i = 0; i < array_length(triangles_to_render); i++)
    // {
    //     printf("triangle %d avg_depth = %f\n", i, triangles_to_render[i].avg_depth);
    // }
}

void free_resources(void)
{
    free(color_buffer);
    array_free(mesh.vertices);
    array_free(mesh.faces);
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

    int num_triangles = array_length(triangles_to_render);

    for (int i = 0; i < num_triangles; i++)
    {

        triangle_t triangle = triangles_to_render[i];

        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            draw_filled_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color);
        }

        if (render_method == RENDER_WIRE_VERTEX || render_method == RENDER_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                0xFFFFFFFF);
        }

        if (render_method == RENDER_VERTEX || render_method == RENDER_WIRE_VERTEX)
        {
            draw_rect(
                triangle.points[0].x - 3,
                triangle.points[0].y - 3,
                6,
                6,
                0xFFFFFFFF);

            draw_rect(
                triangle.points[1].x - 3,
                triangle.points[1].y - 3,
                6,
                6,
                0xFFFFFFFF);

            draw_rect(
                triangle.points[2].x - 3,
                triangle.points[2].y - 3,
                6,
                6,
                0xFFFFFFFF);
        }
    }

    // draw_triangle(0, 0, 0, 300, 300, 100, 0xFFFFFFFF);
    // draw_filled_triangle(0, 0, 0, 280, 230, 100, 0xFF0000FF);

    draw_grid(55, 0xFFFFFFFF);

    // draw_pixel(406, 406, 0xFFFF0000);

    // draw_rect(0, 0, 200, 200, 0xFFFF0000);

    array_free(triangles_to_render);

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
    free_resources();

    return 0;
}