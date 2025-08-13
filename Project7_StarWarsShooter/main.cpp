#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;

struct Entity {
    float x, y;
    float w, h;
    float vx, vy;
};

bool checkCollision(const Entity& a, const Entity& b) {
    return a.x < b.x + b.w && a.x + a.w > b.x &&
           a.y < b.y + b.h && a.y + a.h > b.y;
}

void drawRect(SDL_Renderer* renderer, const Entity& e, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect r{static_cast<int>(e.x), static_cast<int>(e.y),
               static_cast<int>(e.w), static_cast<int>(e.h)};
    SDL_RenderFillRect(renderer, &r);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Init Failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Star Wars Shooter",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(static_cast<unsigned>(time(nullptr)));

    Entity player{WIDTH / 2.f - 20, HEIGHT - 60.f, 40, 40, 0, 0};
    std::vector<Entity> bullets;
    std::vector<Entity> enemies;
    std::vector<Entity> asteroids;

    Entity boss{-100, 50, 120, 60, 2, 0};
    bool bossActive = false;
    int bossHealth = 20;

    int spawnTimer = 0;
    Uint32 lastShot = 0;
    bool running = true;
    SDL_Event event;
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (keystate[SDL_SCANCODE_LEFT]) player.x -= 5;
        if (keystate[SDL_SCANCODE_RIGHT]) player.x += 5;
        if (player.x < 0) player.x = 0;
        if (player.x > WIDTH - player.w) player.x = WIDTH - player.w;

        if (keystate[SDL_SCANCODE_SPACE]) {
            if (SDL_GetTicks() - lastShot > 250) {
                bullets.push_back({player.x + player.w/2 - 2, player.y - 10, 4, 10, 0, -8});
                lastShot = SDL_GetTicks();
            }
        }

        spawnTimer++;
        if (spawnTimer % 50 == 0 && !bossActive) {
            enemies.push_back({static_cast<float>(rand() % (WIDTH - 40)), -40.f, 40, 40, 0, 3});
        }
        if (spawnTimer % 120 == 0 && !bossActive) {
            asteroids.push_back({static_cast<float>(rand() % (WIDTH - 30)), -30.f, 30, 30, 0, 4});
        }
        if (spawnTimer > 1500 && !bossActive) {
            bossActive = true;
            boss.x = WIDTH/2 - boss.w/2;
        }

        for (auto& b : bullets) b.y += b.vy;
        for (auto& e : enemies) e.y += e.vy;
        for (auto& a : asteroids) a.y += a.vy;

        if (bossActive) {
            boss.x += boss.vx;
            if (boss.x <= 0 || boss.x >= WIDTH - boss.w) boss.vx = -boss.vx;
        }

        for (size_t i = 0; i < bullets.size(); ) {
            bool removed = false;
            for (size_t j = 0; j < enemies.size(); ) {
                if (checkCollision(bullets[i], enemies[j])) {
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    removed = true;
                    break;
                } else {
                    ++j;
                }
            }
            if (removed) continue;
            if (bossActive && checkCollision(bullets[i], boss)) {
                bullets.erase(bullets.begin() + i);
                if (--bossHealth <= 0) {
                    bossActive = false;
                    running = false; // player wins
                }
                continue;
            }
            if (bullets[i].y < -bullets[i].h) {
                bullets.erase(bullets.begin() + i);
            } else {
                ++i;
            }
        }

        for (const auto& e : enemies) {
            if (checkCollision(e, player)) running = false; // player hit
        }
        for (const auto& a : asteroids) {
            if (checkCollision(a, player)) running = false;
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](const Entity& e){return e.y > HEIGHT; }),
                      enemies.end());
        asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
                                       [](const Entity& e){return e.y > HEIGHT; }),
                        asteroids.end());

        SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
        SDL_RenderClear(renderer);

        drawRect(renderer, player, {0, 255, 0, 255});
        for (const auto& b : bullets) drawRect(renderer, b, {255, 255, 0, 255});
        for (const auto& e : enemies) drawRect(renderer, e, {255, 0, 0, 255});
        for (const auto& a : asteroids) drawRect(renderer, a, {150, 150, 150, 255});
        if (bossActive) drawRect(renderer, boss, {0, 0, 255, 255});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
