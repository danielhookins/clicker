#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib> 
#include <iostream>
#include <sstream>
#include <filesystem>

const int BOX_WIDTH = 50;
const int BOX_HEIGHT = 50;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BOX_SPEED = 80.0f;

int score = 0;  // Scoreboard variable
const char* fontPath = nullptr;
TTF_Font* font = nullptr;

struct Box {
    float x, y;
    float dx, dy;
    SDL_Color color;

    Box() : x(WINDOW_WIDTH / 2 - BOX_WIDTH / 2), y(WINDOW_HEIGHT / 2 - BOX_HEIGHT / 2), dx(BOX_SPEED), dy(BOX_SPEED), color{0, 0, 255, 255} {}

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

void update(float deltaTime) {
    box.moveAndBounce(deltaTime);
}

void input(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= box.x && mouseX <= box.x + BOX_WIDTH && mouseY >= box.y && mouseY <= box.y + BOX_HEIGHT) {
            box.color.r = rand() % 256;
            box.color.g = rand() % 256;
            box.color.b = rand() % 256;
            score++;
        }
    }
}

void draw(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(renderer);

    // Draw the box
    SDL_Rect rect = { static_cast<int>(box.x), static_cast<int>(box.y), BOX_WIDTH, BOX_HEIGHT };
    SDL_SetRenderDrawColor(renderer, box.color.r, box.color.g, box.color.b, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Draw the scoreboard
    SDL_Color textColor = {0, 0, 0, 255};  // Black color
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect textRect = { WINDOW_WIDTH - textWidth - 10, 10, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf Initialization Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    fontPath = "assets/fonts/open-sans/OpenSans-ExtraBold.ttf";
    font = TTF_OpenFont(fontPath, 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        std::cout << "Font file NOT found at: " << fontPath << std::endl;
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
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

    int running = 1;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            input(event);
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        update(deltaTime);
        draw(window, renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
