#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FORT_SIZE = 80;
const int UNIT_SIZE = 20;
const int UNIT_SPEED = 2;

struct Building {
    SDL_Rect rect;
    int health = 100;
    bool exists = false;
    SDL_Color color;
};

struct Unit {
    SDL_Rect rect;
    bool enemy;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Cossacks Style Strategy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Building playerFort;
    playerFort.color = {0, 0, 255, 255};
    Building enemyFort;
    enemyFort.color = {255, 0, 0, 255};
    enemyFort.rect = {SCREEN_WIDTH - FORT_SIZE - 20, SCREEN_HEIGHT / 2 - FORT_SIZE / 2, FORT_SIZE, FORT_SIZE};
    enemyFort.exists = true;

    std::vector<Unit> units;
    bool running = true;
    int enemySpawnTimer = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && !playerFort.exists) {
                int x = e.button.x - FORT_SIZE / 2;
                int y = e.button.y - FORT_SIZE / 2;
                playerFort.rect = {x, y, FORT_SIZE, FORT_SIZE};
                playerFort.exists = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s && playerFort.exists) {
                Unit u;
                u.enemy = false;
                u.rect = {playerFort.rect.x + playerFort.rect.w, playerFort.rect.y + playerFort.rect.h / 2 - UNIT_SIZE / 2, UNIT_SIZE, UNIT_SIZE};
                units.push_back(u);
            }
        }

        // Spawn enemy units periodically
        enemySpawnTimer++;
        if (enemySpawnTimer > 120 && enemyFort.exists) {
            enemySpawnTimer = 0;
            Unit u;
            u.enemy = true;
            u.rect = {enemyFort.rect.x - UNIT_SIZE, enemyFort.rect.y + enemyFort.rect.h / 2 - UNIT_SIZE / 2, UNIT_SIZE, UNIT_SIZE};
            units.push_back(u);
        }

        // Move units
        for (auto& u : units) {
            if (u.enemy)
                u.rect.x -= UNIT_SPEED;
            else
                u.rect.x += UNIT_SPEED;
        }

        // Check collisions between units
        for (size_t i = 0; i < units.size(); ++i) {
            for (size_t j = i + 1; j < units.size(); ++j) {
                if (units[i].enemy != units[j].enemy && SDL_HasIntersection(&units[i].rect, &units[j].rect)) {
                    units.erase(units.begin() + j);
                    units.erase(units.begin() + i);
                    i--;
                    break;
                }
            }
        }

        // Units attacking forts
        for (size_t i = 0; i < units.size(); ++i) {
            if (!units[i].enemy && enemyFort.exists && SDL_HasIntersection(&units[i].rect, &enemyFort.rect)) {
                enemyFort.health -= 10;
                units.erase(units.begin() + i);
                i--;
                if (enemyFort.health <= 0) enemyFort.exists = false;
            } else if (units[i].enemy && playerFort.exists && SDL_HasIntersection(&units[i].rect, &playerFort.rect)) {
                playerFort.health -= 10;
                units.erase(units.begin() + i);
                i--;
                if (playerFort.health <= 0) playerFort.exists = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        if (playerFort.exists) {
            SDL_SetRenderDrawColor(renderer, playerFort.color.r, playerFort.color.g, playerFort.color.b, 255);
            SDL_RenderFillRect(renderer, &playerFort.rect);
        }

        if (enemyFort.exists) {
            SDL_SetRenderDrawColor(renderer, enemyFort.color.r, enemyFort.color.g, enemyFort.color.b, 255);
            SDL_RenderFillRect(renderer, &enemyFort.rect);
        }

        for (auto& u : units) {
            if (u.enemy)
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(renderer, &u.rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
