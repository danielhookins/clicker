#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib> 
#include <iostream>
#include <sstream>
#include <filesystem>
#include <random>

// Constants for game configuration
constexpr int BOX_WIDTH = 50;
constexpr int BOX_HEIGHT = 50;
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr float BOX_SPEED = 80.0f;

/**
 * The main Game class that encapsulates game logic, input handling, and rendering.
 */
class Game {
private:
    int score = 0;  // Current game score
    TTF_Font* font = nullptr;  // Font used for rendering the score
    SDL_Renderer* renderer = nullptr;  // SDL Renderer instance

    /**
     * Nested Box structure representing the clickable box in the game.
     */
    struct Box {
        float x, y;  // Position of the box
        float dx, dy;  // Movement direction and speed
        SDL_Color color;  // Color of the box

        // Constructor initializes the box in the center of the window
        Box() : x(WINDOW_WIDTH / 2 - BOX_WIDTH / 2), y(WINDOW_HEIGHT / 2 - BOX_HEIGHT / 2), dx(BOX_SPEED), dy(BOX_SPEED), color{0, 0, 255, 255} {}

        /**
         * Moves the box and makes it bounce when it hits window boundaries.
         * @param deltaTime Time since the last frame.
         */
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
    } box;  // Instance of the Box

public:
    /**
     * Constructor initializes the game with the given renderer.
     * @param renderer SDL Renderer instance.
     */
    Game(SDL_Renderer* renderer) : renderer(renderer) {
        font = TTF_OpenFont("assets/fonts/open-sans/OpenSans-ExtraBold.ttf", 24);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        }
    }

    /**
     * Destructor ensures resources are properly deallocated.
     */
    ~Game() {
        if (font) {
            TTF_CloseFont(font);
        }
    }

    /**
     * Updates the game state.
     * @param deltaTime Time since the last frame.
     */
    void update(float deltaTime) {
        box.moveAndBounce(deltaTime);
    }

    /**
     * Handles input events like mouse clicks.
     * @param event SDL Event instance.
     */
    void input(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= box.x && mouseX <= box.x + BOX_WIDTH && mouseY >= box.y && mouseY <= box.y + BOX_HEIGHT) {
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, 255);
                box.color.r = dist(mt);
                box.color.g = dist(mt);
                box.color.b = dist(mt);
                score++;
            }
        }
    }

    /**
     * Renders the game state to the window.
     */
    void draw() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set background color
        SDL_RenderClear(renderer);  // Clear the renderer

        // Render the box
        SDL_Rect rect = { static_cast<int>(box.x), static_cast<int>(box.y), BOX_WIDTH, BOX_HEIGHT };
        SDL_SetRenderDrawColor(renderer, box.color.r, box.color.g, box.color.b, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Render the score
        SDL_Color textColor = {0, 0, 0, 255};  // Text color
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int textWidth = textSurface->w;
        int textHeight = textSurface->h;
        SDL_FreeSurface(textSurface);
        SDL_Rect textRect = { WINDOW_WIDTH - textWidth - 10, 10, textWidth, textHeight };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(renderer);  // Present the rendered frame
    }
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf Initialization Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Clicker",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Game game(renderer);  // Create game instance

    int running = 1;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            game.input(event);
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        game.update(deltaTime);
        game.draw();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
