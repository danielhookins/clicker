#include <SDL_main.h>
#include <SDL.h>

int SDL_main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    SDL_Window* window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        160,                               // width, in pixels
        144,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // The window is open: enter program loop (see SDL_PollEvent)
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Check for the quit event. SDL_Quit() is implicitly called by SDL_DestroyWindow()
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
