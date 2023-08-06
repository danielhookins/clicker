#include <SDL.h>

const int BOX_WIDTH = 50;
const int BOX_HEIGHT = 50;
const int WINDOW_WIDTH = 160;
const int WINDOW_HEIGHT = 144;
const float BOX_SPEED = 80.0f;

struct Box {
    float x, y;
    float dx, dy;

    Box() : x(WINDOW_WIDTH / 2 - BOX_WIDTH / 2), y(WINDOW_HEIGHT / 2 - BOX_HEIGHT / 2), dx(BOX_SPEED), dy(BOX_SPEED) {}

    void moveAndBounce(float deltaTime) {
        x += dx * deltaTime;
        y += dy * deltaTime;

        if (x <= 0 || x + BOX_WIDTH >= WINDOW_WIDTH) {
            dx = -dx;
        }
        if (y <= 0 || y + BOX_HEIGHT >= WINDOW_HEIGHT) {
            dy = -dy;
        }
    }
};

Box box;

// Function to update game state
void update(float deltaTime) {
    box.moveAndBounce(deltaTime);
}

// Function to handle input
void input(SDL_Event& event) {
    // Existing input handling code...
}

// Function to draw to the screen
void draw(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(renderer);

    // Draw the box
    SDL_Rect rect = { static_cast<int>(box.x), static_cast<int>(box.y), BOX_WIDTH, BOX_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue box
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    SDL_Window* window = SDL_CreateWindow(
        "Clicker",                         // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH,                      // width, in pixels
        WINDOW_HEIGHT,                     // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // The window is open: enter program loop (see SDL_PollEvent)
    int running = 1;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        // Update Delta Time
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;  // Convert to seconds
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            input(event);
            
            // Check for the quit message
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        update(deltaTime);
        draw(window);
    }
    
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    
    return 0;
}
