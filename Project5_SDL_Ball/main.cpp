#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Bouncing Ball",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    int x = WIDTH/2, y = HEIGHT/2;
    int vx = 5, vy = 5;
    int radius = 20;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        x += vx;
        y += vy;

        if (x <= radius || x >= WIDTH - radius) vx = -vx;
        if (y <= radius || y >= HEIGHT - radius) vy = -vy;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
        SDL_Rect ball = {x - radius, y - radius, radius * 2, radius * 2};
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}