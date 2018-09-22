#ifndef _PLAY_MODE_H_
#define _PLAY_MODE_H_

#include "entities.h"
#include "game_mode.h"

#include <SDL2/SDL.h>

#include <vector>

struct SpawnInfo
{
    u32 amount;
    u32 cooldown_ms;
    u32 time_last_spawned = 0;
};

enum struct Side
{
    LEFT   = 0,
    RIGHT  = 1,
    TOP    = 2,
    BOTTOM = 3
};


struct PlayMode: GameMode
{
    virtual void HandleEvents(GameState* state) override;
    virtual void Update(GameState* state) override;
    virtual void Render(Renderer* renderer, const GameState& state) override;

    SDL_Event events;
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<Blaster> blasters;
    std::vector<Drone> drones;
    std::vector<Laser> player_lasers;
    std::vector<Laser> blaster_lasers;

    SpawnInfo spawn_info_asteroid = {5, 5000};
    SpawnInfo spawn_info_blaster = {1, 8000};
    SpawnInfo spawn_info_drone = {5, 1500};


    Texture texture_player {"../assets/sprites/playership.png"};
    Texture texture_asteroid{"../assets/sprites/meteor.png"};
    Texture texture_blaster{"../assets/sprites/blaster.png"};
    Texture texture_drone{"../assets/sprites/drone.png"};
    Texture texture_player_laser{"../assets/sprites/playerlaser.png"};
    Texture texture_enemy_laser{"../assets/sprites/enemylaser.png"};
    Texture texture_background{"../assets/sprites/space.png"};


    std::random_device random;
    std::uniform_int_distribution<int> range_x{0, g_screen_width};
    std::uniform_int_distribution<int> range_y{0, g_screen_height};


private:
    void UpdatePlayer();
    void UpdateLasers();
    void UpdateEntities();
    void SpawnEnemies();
    void ResolveCollisions(GameState* state);
    void RemoveDeadEntities();
    Rectangle GenerateSpawnPosition(float width, float height);
    void SpawnAsteroids();
    void SpawnBlasters();
    void SpawnDrones();
    Point GeneratePointOnOppositeSide(Point origin);
};

#endif // _PLAY_MODE_H_
