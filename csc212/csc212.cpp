<<<<<<< HEAD
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool init(SDL_Window** window, SDL_Renderer** renderer);
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* texture1, SDL_Texture* texture2, SDL_Texture* frameTexture);

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        std::cerr << "Failed to initialize!\n";
        return -1;
    }

    SDL_Texture* backgroundTexture = loadTexture("./assets/layer2.png", renderer);
    SDL_Texture* texture1 = loadTexture("./assets/layer1.jpeg", renderer);
    SDL_Texture* texture2 = loadTexture("./assets/layer0.jpeg", renderer);
    SDL_Texture* frameTexture = loadTexture("./assets/layer3.png", renderer); // Load the frame texture

    if (backgroundTexture == nullptr || texture1 == nullptr || texture2 == nullptr || frameTexture == nullptr) {
        std::cerr << "Failed to load textures!\n";
        close(window, renderer, backgroundTexture, texture1, texture2, frameTexture);
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        // Render background texture
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        // Set positions and sizes for the textures
        int textureWidth = 200;
        int textureHeight = 200;
        int spaceBetween = 20;

        SDL_Rect frameQuad1 = {
            (WINDOW_WIDTH / 2) - textureWidth - (spaceBetween / 2),
            (WINDOW_HEIGHT / 2) - (textureHeight / 2),
            textureWidth,
            textureHeight
        };

        SDL_Rect frameQuad2 = {
            (WINDOW_WIDTH / 2) + (spaceBetween / 2),
            (WINDOW_HEIGHT / 2) - (textureHeight / 2),
            textureWidth,
            textureHeight
        };

        SDL_Rect renderQuad1 = {
            frameQuad1.x + 10, // Adjust these offsets according to your frame
            frameQuad1.y + 10, // Adjust these offsets according to your frame
            textureWidth - 20, // Adjust these sizes according to your frame
            textureHeight - 20 // Adjust these sizes according to your frame
        };

        SDL_Rect renderQuad2 = {
            frameQuad2.x + 10, // Adjust these offsets according to your frame
            frameQuad2.y + 10, // Adjust these offsets according to your frame
            textureWidth - 20, // Adjust these sizes according to your frame
            textureHeight - 20 // Adjust these sizes according to your frame
        };

        // Render the frame textures
        SDL_RenderCopy(renderer, frameTexture, nullptr, &frameQuad1);
        SDL_RenderCopy(renderer, frameTexture, nullptr, &frameQuad2);

        // Render the images on top of the frames
        SDL_RenderCopy(renderer, texture1, nullptr, &renderQuad1);
        SDL_RenderCopy(renderer, texture2, nullptr, &renderQuad2);

        SDL_RenderPresent(renderer);
    }

    close(window, renderer, backgroundTexture, texture1, texture2, frameTexture);
    return 0;
}

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << "\n";
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << "\n";
        return nullptr;
    }

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << "\n";
    }

    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* texture1, SDL_Texture* texture2, SDL_Texture* frameTexture) {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    backgroundTexture = nullptr;
    texture1 = nullptr;
    texture2 = nullptr;
    frameTexture = nullptr;
    renderer = nullptr;
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}
=======
#include "csc212.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int speed = 0.1;
int CameraX = 0;
int textureWidthDiff = IMAGE_WIDTH - SCREEN_WIDTH;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    std::vector<SDL_Texture*> backgroundTextures;
    backgroundTextures.push_back(loadTexture("./ssets/layer0.png"));
    backgroundTextures.push_back(loadTexture("./assets/layer1.png"));
  



    SDL_Texture* backgroundFloor = loadTexture("./Assets/grassfloor.png");
    SDL_Texture* backgroundSky = loadTexture("./Assets/skytoon.jpg");

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        //SDL_Rect skyDest = { 0, -400, 800, 810 };
        //SDL_RenderCopy(gRenderer, backgroundSky, NULL, &skyDest);

        //SDL_RenderCopy(gRenderer, backgroundFloor, NULL, NULL);

        for (SDL_Texture* texture : backgroundTextures) {
            SDL_Rect textureDest = { 0, 0, IMAGE_WIDTH, SCREEN_HEIGHT };
            SDL_RenderCopy(gRenderer, texture, NULL, &textureDest);
        }

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Destroy textures
    //SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(backgroundFloor);
    SDL_DestroyTexture(backgroundSky);

    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }

    // Close SDL
    closeSDL();

    return 0;
}
>>>>>>> 20b7c7f14cdf97d21ea0c379a03aaeeeb550c610
