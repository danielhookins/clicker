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

// Progress Bar
const int TOTAL_CLICKS = 100;

constexpr int PROGRESS_BAR_HEIGHT = 10;  // Height of the progress bar
constexpr int PROGRESS_BAR_OFFSET = 10;  // Offset from the box

/**
 * The main Game class that encapsulates game logic, input handling, and rendering.
 */
class Game {
private:
    int score = 0;  // Current game score
    TTF_Font* font = nullptr;  // Font used for rendering the score
    SDL_Renderer* renderer = nullptr;  // SDL Renderer instance
    int numBoxes = 3;  // Number of boxes in the game

    /**
     * Nested Box structure representing the clickable box in the game.
     */
    struct Box {
        // Box Properties
        float x, y;  // Position of the box
        float dx, dy;  // Movement direction and speed
        SDL_Color color;  // Color of the box

        // Click percentage / Box damage
        int progressPercentage = 0;

        // Shake effect
        float shakeTime = 0.0f;  // Time left for shaking
        static constexpr float SHAKE_DURATION = 0.1f; // Duration of the shake in seconds
        static const float SHAKE_INTENSITY;  // Intensity of the shake in pixels

        // Constructor initializes the box at a random position within the window
        Box() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> distX(0, WINDOW_WIDTH - BOX_WIDTH);
            std::uniform_real_distribution<float> distY(PROGRESS_BAR_HEIGHT + PROGRESS_BAR_OFFSET, WINDOW_HEIGHT - BOX_HEIGHT);
            
            x = distX(mt);
            y = distY(mt);
            
            dx = BOX_SPEED;
            dy = BOX_SPEED;
            color = {0, 0, 255, 255};
        }

        /**
         * Moves the box and makes it bounce when it hits window boundaries.
         * @param deltaTime Time since the last frame.
         */
        void moveAndBounce(float deltaTime) {
            x += dx * deltaTime;
            y += dy * deltaTime;

            if (shakeTime > 0.0f) {
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<float> dist(-SHAKE_INTENSITY, SHAKE_INTENSITY);
                x += dist(mt);
                y += dist(mt);
                shakeTime -= deltaTime;
            }

            if (x <= 0 || x + BOX_WIDTH >= WINDOW_WIDTH) {
                dx = -dx;
            }
            if (y <= (0 + PROGRESS_BAR_HEIGHT + PROGRESS_BAR_OFFSET) || y + BOX_HEIGHT >= WINDOW_HEIGHT) {
                dy = -dy;
            }
        }
    };
    
    std::vector<Box> boxes;  // Vector of boxes

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
        boxes.resize(numBoxes);  // Initialize the vector with the desired number of boxes
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
        for (auto& box : boxes) {
            box.moveAndBounce(deltaTime);
        }
    }

    /**
     * Handles input events like mouse clicks.
     * @param event SDL Event instance.
     */
    void input(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            for (auto& box : boxes) {
                if (mouseX >= box.x && mouseX <= box.x + BOX_WIDTH && mouseY >= box.y && mouseY <= box.y + BOX_HEIGHT) {
                    std::random_device rd;
                    std::mt19937 mt(rd());
                    std::uniform_int_distribution<int> dist(0, 255);
                    box.color.r = dist(mt);
                    box.color.g = dist(mt);
                    box.color.b = dist(mt);
                    
                    // Give the box a shake effect
                    box.shakeTime = box.SHAKE_DURATION;

                    // Increase the score
                    score++;

                    // Update the progress of the clicked box
                    if (box.progressPercentage < 100) {
                        box.progressPercentage += 1;  // Increment by 1 or any other value you prefer
                        if (box.progressPercentage > 100) {
                            box.progressPercentage = 100;
                        }
                    }
                }
            }
        }

        // Remove boxes that have reached 100% progress
        boxes.erase(std::remove_if(boxes.begin(), boxes.end(), [](const Box& box) {
            return box.progressPercentage >= 100;
        }), boxes.end());
    }


    /**
     * Renders the game state to the window.
     */
    void draw() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (const auto& box : boxes) {
            // Show progress bar only if it's not 100%
            if (box.progressPercentage < 100 && box.progressPercentage > 0) {
                // Render the progress bar background
                SDL_Rect progressBarBackground = { static_cast<int>(box.x), static_cast<int>(box.y) - PROGRESS_BAR_HEIGHT - PROGRESS_BAR_OFFSET, BOX_WIDTH, PROGRESS_BAR_HEIGHT };
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &progressBarBackground);

                // Render the progress bar fill
                SDL_Rect progressBarFill = { static_cast<int>(box.x), static_cast<int>(box.y) - PROGRESS_BAR_HEIGHT - PROGRESS_BAR_OFFSET, BOX_WIDTH * box.progressPercentage / 100, PROGRESS_BAR_HEIGHT };
                SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
                SDL_RenderFillRect(renderer, &progressBarFill);
            }

            // Render the box
            SDL_Rect rect = { static_cast<int>(box.x), static_cast<int>(box.y), BOX_WIDTH, BOX_HEIGHT };
            SDL_SetRenderDrawColor(renderer, box.color.r, box.color.g, box.color.b, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

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

const float Game::Box::SHAKE_INTENSITY = 5.0f;

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
