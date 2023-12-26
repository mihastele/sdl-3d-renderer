#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool initialize_window(void)
{
    /* Initialize SDL */

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          480,
                                          0);

    return true;
}

int main(int argc, char *argv[])
{
    /* Create SDL window */

    return 0;
}