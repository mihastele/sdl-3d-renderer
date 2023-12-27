#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

bool is_running = false;

#define N_POINTS (9 * 9 * 9)
float fov_factor = 640;

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

void setup(void)
{
    /* Add setup code here */
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

    int point_count = 0;

    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
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
    /* Add update code here */
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t point = cube_points[i];

        point.z -= camera_position.z;

        vec2_t projected_point = project(point);
        // int x = (projected_point.x + 1.0) * (window_width / 2.0);
        // int y = (projected_point.y + 1.0) * (window_height / 2.0);

        // draw_rect(x, y, 4, 4, 0xFFFFFFFF);
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    // we are using color buffer now
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderClear(renderer);

    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        // int x = (projected_points[i].x + 1.0) * (window_width / 2.0);
        // int y = (projected_points[i].y + 1.0) * (window_height / 2.0);

        draw_rect(
            projected_point.x + window_width / 2,
            projected_point.y + window_height / 2,
            4,
            4,
            0xFFFFFF00);
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