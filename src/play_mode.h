#ifndef _PLAY_MODE_H_
#define _PLAY_MODE_H_

#include "entities.h"
#include "game_mode.h"

#include <SDL2/SDL.h>

#include <vector>

struct SpawnInfo
{
    u32 width;
    u32 height;
    u32 amount;
    u32 delay_ms;
    u32 time_last_spawned = 0;
};


struct EnemyValue
{
    int asteroid = 50;
    int drone = 5;
    int blaster = 25;
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

    EnemyValue enemy_values;

    SpawnInfo spawn_info_asteroid = {50, 50, 10, 5000};
    SpawnInfo spawn_info_blaster = {75, 75, 10, 5000};
    SpawnInfo spawn_info_drone = {75, 75, 5, 500};


private:
    void UpdatePlayer();
    void UpdateLasers();
    void UpdateEnemies();
    void SpawnEnemies();
    void ResolveCollisions(GameState* state);
    void RemoveDeadEntities();
    Point GenerateSpawnPosition(const SpawnInfo& info);
};

#endif // _PLAY_MODE_H_
