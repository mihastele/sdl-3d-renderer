#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

bool initialize_window(void)
{
    /* Initialize SDL */

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(NULL,                   // no title, fullscreen
                              SDL_WINDOWPOS_CENTERED, // centered x
                              SDL_WINDOWPOS_CENTERED, // centered y
                              800,                    // width
                              600,                    // height
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

    return true;
}

int main(int argc, char *argv[])
{
    /* Create SDL window */

    return 0;
}