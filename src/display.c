
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

int window_width = 800;
int window_height = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

bool initialize_window(void)
{
    /* Initialize SDL */

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    // query window size
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(NULL,                   // no title, fullscreen
                              SDL_WINDOWPOS_CENTERED, // centered x
                              SDL_WINDOWPOS_CENTERED, // centered y
                              window_width,           // width
                              window_height,          // height
                              SDL_WINDOW_BORDERLESS); // no flags

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, // Window to create renderer for
                                  -1,     // Display device
                                  0);     // Renderer flags

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    return true;
}

void draw_grid(int grid_size, uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            if (x % grid_size == 0 || y % grid_size == 0)
            {
                color_buffer[(window_width * y) + x] = color;
            }
        }
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y >= 0 && y < window_height)
        color_buffer[(window_width * y) + x] = color;
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_filled_triangle(
    int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    if (y0 > y1)
    {
        int temp = y0;
        y0 = y1;
        y1 = temp;
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if (y1 > y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y0 > y1)
    {
        int temp = y0;
        y0 = y1;
        y1 = temp;
        temp = x0;
        x0 = x1;
        x1 = temp;
    }

    int total_height = y2 - y0;

    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > y1 - y0 || y1 == y0;
        int segment_height = second_half ? y2 - y1 : y1 - y0;

        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? y1 - y0 : 0)) / segment_height;

        int ax = x0 + (x2 - x0) * alpha;
        int ay = y0 + (y2 - y0) * alpha;

        int bx = second_half ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;
        int by = second_half ? y1 + (y2 - y1) * beta : y0 + (y1 - y0) * beta;

        if (ax > bx)
        {
            int temp = ax;
            ax = bx;
            bx = temp;
            temp = ay;
            ay = by;
            by = temp;
        }

        for (int j = ax; j <= bx; j++)
        {
            draw_pixel(j, y0 + i, color);
        }
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            draw_pixel(x + j, y + i, color);
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(window_width * sizeof(uint32_t)));

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void destroy_window(void)
{
    // free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}