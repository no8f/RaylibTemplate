#include <entt/entt.hpp>
#include <raylib.h>
#include <cmath>
#include <iostream>

// Components - Make sure all numeric types are consistent
struct Position { 
    float x, y; 
    Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Velocity { 
    float dx, dy; 
    Velocity(float dx = 0.0f, float dy = 0.0f) : dx(dx), dy(dy) {}
};

struct Renderable { 
    Color color; 
    float radius; 
    Renderable(Color color = WHITE, float radius = 10.0f) : color(color), radius(radius) {}
};

struct Player { 
    float speed; 
    Player(float speed = 200.0f) : speed(speed) {}
};

struct Enemy { 
    float health; 
    Enemy(float health = 100.0f) : health(health) {}
};

class Game {
private:
    entt::registry registry;
    const int screenWidth = 800;
    const int screenHeight = 600;

public:
    Game() {
        InitWindow(screenWidth, screenHeight, "EnTT + Raylib Game");
        SetTargetFPS(60);
        initializeGame();
    }

    ~Game() {
        CloseWindow();
    }

    void run() {
        while (!WindowShouldClose()) {
            update();
            render();
        }
    }

private:
    entt::entity createPlayer(float x, float y) {
        auto entity = registry.create();
        
        // Use explicit float values or cast
        registry.emplace<Position>(entity, x, y);
        registry.emplace<Velocity>(entity, 0.0f, 0.0f);  // Explicit float literals
        registry.emplace<Renderable>(entity, BLUE, 15.0f);
        registry.emplace<Player>(entity, 200.0f);
        
        return entity;
    }

    entt::entity createEnemy(float x, float y) {
        auto entity = registry.create();
        
        // Cast GetRandomValue results to float
        registry.emplace<Position>(entity, x, y);
        registry.emplace<Velocity>(entity, 
            static_cast<float>(GetRandomValue(-50, 50)), 
            static_cast<float>(GetRandomValue(-50, 50)));
        registry.emplace<Renderable>(entity, RED, 10.0f);
        registry.emplace<Enemy>(entity, 100.0f);
        
        return entity;
    }

    void initializeGame() {
        // Create player
        createPlayer(static_cast<float>(screenWidth) / 2.0f, 
                    static_cast<float>(screenHeight) / 2.0f);
        
        // Create some enemies
        for (int i = 0; i < 5; i++) {
            createEnemy(
                static_cast<float>(GetRandomValue(50, screenWidth - 50)),
                static_cast<float>(GetRandomValue(50, screenHeight - 50))
            );
        }
    }

    void update() {
        inputSystem();
        movementSystem();
        collisionSystem();
        
        // Spawn new enemy occasionally
        if (GetRandomValue(1, 120) == 1) {
            createEnemy(
                static_cast<float>(GetRandomValue(0, screenWidth)),
                static_cast<float>(GetRandomValue(0, screenHeight))
            );
        }
    }

    void render() {
        BeginDrawing();
        ClearBackground(BLACK);
        
        renderSystem();
        
        // Draw UI
        DrawText("Use WASD or Arrow Keys to move", 10, 10, 20, WHITE);
        DrawText(TextFormat("Enemies: %d", static_cast<int>(registry.view<Enemy>().size())), 
                10, 40, 20, WHITE);
        
        EndDrawing();
    }

    void movementSystem() {
        float deltaTime = GetFrameTime();
        auto view = registry.view<Position, Velocity>();
        
        for (auto entity : view) {
            auto& pos = registry.get<Position>(entity);
            auto& vel = registry.get<Velocity>(entity);
            
            pos.x += vel.dx * deltaTime;
            pos.y += vel.dy * deltaTime;
            
            // Bounce off walls for enemies
            if (registry.all_of<Enemy>(entity)) {
                if (pos.x <= 0 || pos.x >= static_cast<float>(screenWidth)) {
                    vel.dx *= -1.0f;
                }
                if (pos.y <= 0 || pos.y >= static_cast<float>(screenHeight)) {
                    vel.dy *= -1.0f;
                }
            }
            
            // Clamp player to screen
            if (registry.all_of<Player>(entity)) {
                if (pos.x < 0) pos.x = 0;
                if (pos.x > static_cast<float>(screenWidth)) pos.x = static_cast<float>(screenWidth);
                if (pos.y < 0) pos.y = 0;
                if (pos.y > static_cast<float>(screenHeight)) pos.y = static_cast<float>(screenHeight);
            }
        }
    }

    void inputSystem() {
        auto playerView = registry.view<Player, Velocity>();
        
        for (auto entity : playerView) {
            auto& vel = registry.get<Velocity>(entity);
            auto& player = registry.get<Player>(entity);
            
            vel.dx = 0.0f;
            vel.dy = 0.0f;
            
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) vel.dy = -player.speed;
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) vel.dy = player.speed;
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) vel.dx = -player.speed;
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) vel.dx = player.speed;
        }
    }

    void collisionSystem() {
        auto players = registry.view<Player, Position, Renderable>();
        auto enemies = registry.view<Enemy, Position, Renderable>();
        
        for (auto player : players) {
            auto& playerPos = registry.get<Position>(player);
            auto& playerRender = registry.get<Renderable>(player);
            
            for (auto enemy : enemies) {
                auto& enemyPos = registry.get<Position>(enemy);
                auto& enemyRender = registry.get<Renderable>(enemy);
                
                float distance = std::sqrt(
                    (playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + 
                    (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y)
                );
                
                if (distance < playerRender.radius + enemyRender.radius) {
                    registry.destroy(enemy);
                    break;
                }
            }
        }
    }

    void renderSystem() {
        auto view = registry.view<Position, Renderable>();
        
        for (auto entity : view) {
            auto& pos = registry.get<Position>(entity);
            auto& render = registry.get<Renderable>(entity);
            
            DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 
                      render.radius, render.color);
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}