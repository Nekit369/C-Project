#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int CAR_WIDTH = 40;
const int CAR_HEIGHT = 80;
const int FINISH_LINE = 50;

struct Car {
    int x, y;
    SDL_Scancode up, down, left, right;
    SDL_Color color;
};

void renderCar(SDL_Renderer* renderer, const Car& car) {
    SDL_Rect rect{car.x, car.y, CAR_WIDTH, CAR_HEIGHT};
    SDL_SetRenderDrawColor(renderer, car.color.r, car.color.g, car.color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Two Player Racer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Car car1{WIDTH / 4 - CAR_WIDTH/2, HEIGHT - CAR_HEIGHT - 10, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D, {255, 0, 0}};
    Car car2{3 * WIDTH / 4 - CAR_WIDTH/2, HEIGHT - CAR_HEIGHT - 10, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, {0, 0, 255}};

    bool running = true;
    SDL_Event event;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    int winner = 0; // 0 = none, 1 = player1, 2 = player2

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (keystates[car1.up]) car1.y -= 5;
        if (keystates[car1.down]) car1.y += 5;
        if (keystates[car1.left]) car1.x -= 5;
        if (keystates[car1.right]) car1.x += 5;

        if (keystates[car2.up]) car2.y -= 5;
        if (keystates[car2.down]) car2.y += 5;
        if (keystates[car2.left]) car2.x -= 5;
        if (keystates[car2.right]) car2.x += 5;

        car1.x = std::max(0, std::min(WIDTH - CAR_WIDTH, car1.x));
        car1.y = std::max(0, std::min(HEIGHT - CAR_HEIGHT, car1.y));
        car2.x = std::max(0, std::min(WIDTH - CAR_WIDTH, car2.x));
        car2.y = std::max(0, std::min(HEIGHT - CAR_HEIGHT, car2.y));

        if (winner == 0) {
            if (car1.y <= FINISH_LINE) winner = 1;
            if (car2.y <= FINISH_LINE) winner = 2;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, FINISH_LINE, WIDTH, FINISH_LINE);

        renderCar(renderer, car1);
        renderCar(renderer, car2);

        if (winner != 0) {
            SDL_Rect msgRect{WIDTH/2 - 150, HEIGHT/2 - 40, 300, 80};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
            SDL_RenderFillRect(renderer, &msgRect);
            // Simple text replacement using rectangles; no fonts for simplicity
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
