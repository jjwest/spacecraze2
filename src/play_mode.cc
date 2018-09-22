#include "play_mode.h"

#include "common.h"
#include "mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <random>

#include <math.h>


void PlayMode::HandleEvents(GameState* state)
{
    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT)
        {
            state->current_mode = Mode::QUIT;
        }
    }

}

void PlayMode::Update(GameState* state)
{
    UpdateEntities();
    ResolveCollisions(state);
    SpawnEnemies();
    RemoveDeadEntities();
}

void PlayMode::UpdateEntities()
{
    player.Update(&player_lasers);

    bool right_mouse_button_pressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    if (right_mouse_button_pressed && player.has_singularity_weapon)
    {
        asteroids.clear();
        blasters.clear();
        drones.clear();
        blaster_lasers.clear();

        player.has_singularity_weapon = false;
    }

    for (auto& asteroid : asteroids)
    {
        asteroid.Update();
    }

    for (auto& blaster : blasters)
    {
        blaster.Update(&blaster_lasers, player.position);
    }

    for (auto& drone : drones)
    {
        drone.Update(player.position);
    }

    for (auto& laser : player_lasers)
    {
        laser.Update();
    }

    for (auto& laser : blaster_lasers)
    {
        laser.Update();
    }
}


void PlayMode::ResolveCollisions(GameState* state)
{
    for (const Asteroid& asteroid : asteroids)
    {
        if (asteroid.position.Intersects(player.position))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }

        for (Drone& drone : drones)
        {
            if (asteroid.position.Intersects(drone.position))
            {
                drone.health = 0;
            }
        }
    }

    for (const Blaster& blaster : blasters)
    {
        if (blaster.position.Intersects(player.position))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }
    }

    for (const Drone& drone : drones)
    {
        if (drone.position.Intersects(player.position))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }
    }

    for (const Laser& laser : blaster_lasers)
    {
        if (laser.position.Intersects(player.position))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }
    }

    for (Laser& laser : player_lasers)
    {
        for (Asteroid& asteroid : asteroids)
        {
            if (laser.position.Intersects(asteroid.position))
            {
                laser.health = 0;
                asteroid.health -= laser.damage;

                if (asteroid.health <= 0)
                {
                    state->player_score += asteroid.score_value;
                }
            }
        }

        for (Blaster& blaster : blasters)
        {
            if (laser.position.Intersects(blaster.position))
            {
                laser.health = 0;
                blaster.health -= laser.damage;

                if (blaster.health <= 0)
                {
                    state->player_score += blaster.score_value;
                }
            }
        }

        for (Drone& drone : drones)
        {
            if (laser.position.Intersects(drone.position))
            {
                laser.health = 0;
                drone.health -= laser.damage;

                if (drone.health <= 0)
                {
                    state->player_score += drone.score_value;
                }
            }
        }
    }
}

void PlayMode::Render(Renderer* renderer, const GameState& state)
{
    renderer->DrawBackground(texture_background);

    for (const Laser& laser : player_lasers)
    {
        renderer->DrawRect(laser.position, texture_player_laser, laser.angle);
    }

    for (const Asteroid& asteroid : asteroids)
    {
        renderer->DrawRect(asteroid.position, texture_asteroid, asteroid.angle);
    }

    for (const Blaster& blaster : blasters)
    {
        renderer->DrawRect(blaster.position, texture_blaster, blaster.angle);
    }

    for (const Drone& drone : drones)
    {
        renderer->DrawRect(drone.position, texture_drone, drone.angle);
    }

    for (const Laser& laser : blaster_lasers)
    {
        renderer->DrawRect(laser.position, texture_enemy_laser, laser.angle);
    }

    renderer->DrawRect(player.position, texture_player, player.angle);
}


bool OutsideScreen(const Rectangle& rect)
{
    return (rect.x + rect.width < 0 ||
            rect.x > g_screen_width ||
            rect.y + rect.height < 0 ||
            rect.y > g_screen_height);
}

void PlayMode::RemoveDeadEntities()
{
    auto IsDead = [](auto& obj) { return obj.health <= 0.0 || OutsideScreen(obj.position); };

    asteroids.erase(std::remove_if(begin(asteroids), end(asteroids), IsDead), end(asteroids));
    blasters.erase(std::remove_if(begin(blasters), end(blasters), IsDead), end(blasters));
    drones.erase(std::remove_if(begin(drones), end(drones), IsDead), end(drones));
    player_lasers.erase(std::remove_if(begin(player_lasers), end(player_lasers), IsDead), end(player_lasers));
}


Rectangle PlayMode::GenerateSpawnPosition(float width, float height)
{
    static std::uniform_int_distribution<int> spawn_side(0, 3);
    static std::uniform_int_distribution<int> spawn_range_x(0 - width, g_screen_width);
    static std::uniform_int_distribution<int> spawn_range_y(0 - height, g_screen_height);

    Side side = static_cast<Side>(spawn_side(random));
    Rectangle spawn_pos;
    spawn_pos.width = width;
    spawn_pos.height = height;

    switch (side) {
    case Side::LEFT: {
        spawn_pos.x = 0 - width;
        spawn_pos.y = spawn_range_y(random);
        break;
    }
    case Side::RIGHT: {
        spawn_pos.x = g_screen_width;
        spawn_pos.y = spawn_range_y(random);
        break;
    }
    case Side::BOTTOM: {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = g_screen_height;
        break;
    }
    case Side::TOP: {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = 0 - height;
        break;
    }
    };

    return spawn_pos;
}


void PlayMode::SpawnEnemies()
{
    SpawnAsteroids();
    SpawnBlasters();
    SpawnDrones();
}

Point PlayMode::GeneratePointOnOppositeSide(Point origin)
{
    Point destination;

    if (origin.x < g_screen_width / 2)
    {
        destination.x = g_screen_width;
        destination.y = range_y(random);
    }
    else if (origin.x >= g_screen_width)
    {
        destination.x = 0;
        destination.y = range_y(random);
    }
    if (origin.y < g_screen_height / 2)
    {
        destination.y = g_screen_height;
        destination.x = range_x(random);
    }
    else if (origin.y >= g_screen_height)
    {
        destination.y = 0;
        destination.x = range_x(random);
    }

    return destination;
}

void PlayMode::SpawnAsteroids()
{
    SpawnInfo* spawn = &spawn_info_asteroid;
    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms)
    {
        for (u32 i = 0; i < spawn->amount; ++i)
        {
            Asteroid asteroid;
            asteroid.position = GenerateSpawnPosition(asteroid.position.width, asteroid.position.height);
            Point destination = GeneratePointOnOppositeSide(asteroid.position);
            auto [dx, dy] = CalculateMovementDeltas(asteroid.position, destination);
            asteroid.dx = dx;
            asteroid.dy = dy;

            asteroids.push_back(asteroid);
        }

        spawn->time_last_spawned = current_time;
    }
}

void PlayMode::SpawnDrones()
{
    SpawnInfo* spawn = &spawn_info_drone;

    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms)
    {
        for (u32 i = 0; i < spawn->amount; ++i)
        {
            Drone drone;
            drone.position = GenerateSpawnPosition(drone.position.width, drone.position.height);
            drones.push_back(drone);
        }

        spawn->time_last_spawned = current_time;
    }
}

void PlayMode::SpawnBlasters()
{
    SpawnInfo* spawn = &spawn_info_blaster;

    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms)
    {
        for (u32 i = 0; i < spawn->amount; ++i)
        {
            Blaster blaster;
            blaster.position = GenerateSpawnPosition(blaster.position.width, blaster.position.height);
            blasters.push_back(blaster);
        }

        spawn->time_last_spawned = current_time;
    }


}
