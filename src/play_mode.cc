#include "play_mode.h"

#include "platform.h"
#include "mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <random>


void PlayMode::HandleEvents(GameState *state)
{
    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT)
        {
            state->current_mode = Mode::QUIT;
        }
    }

}

void PlayMode::Update(GameState *state)
{
    UpdatePlayer();
    UpdateLasers();
    UpdateEnemies();
    ResolveCollisions(state);
    SpawnEnemies();
    RemoveDeadEntities();
}

void PlayMode::UpdatePlayer()
{
    { // Update movement
        auto key_pressed = SDL_GetKeyboardState(NULL);

        bool diagonal_movement =
            (key_pressed[SDL_SCANCODE_W] || key_pressed[SDL_SCANCODE_S]) &&
            (key_pressed[SDL_SCANCODE_A] || key_pressed[SDL_SCANCODE_D]);

        float speed = diagonal_movement ? player.speed * 0.75 : player.speed;

        bool can_move_up = player.position.y - speed >= 0;
        if (key_pressed[SDL_SCANCODE_W] && can_move_up)
        {
            player.position.y -= speed;
        }

        bool can_move_down = player.position.y + player.hitbox.height + speed <= SCREEN_HEIGHT;
        if (key_pressed[SDL_SCANCODE_S] && can_move_down)
        {
            player.position.y += speed;
        }

        bool can_move_left = player.position.x - speed >= 0;
        if (key_pressed[SDL_SCANCODE_A] && can_move_left)
        {
            player.position.x -= speed;
        }

        bool can_move_right = player.position.x + player.hitbox.width + speed <= SCREEN_WIDTH;
        if (key_pressed[SDL_SCANCODE_D] && can_move_right)
        {
            player.position.x += speed;
        }

        player.hitbox.x = round(player.position.x);
        player.hitbox.y = round(player.position.y);
    }

    Point mouse_pos;

    { // Shoot
        bool left_mouse_button_pressed = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y) & SDL_BUTTON(SDL_BUTTON_LEFT);
        auto current_time = SDL_GetTicks();
        if (left_mouse_button_pressed &&
            (current_time - player.time_last_shot_ms > player.shoot_cooldown_ms))
        {
            Point laser_origin;
            // We hack the positions a bit to ensure the laser originates from a good
            // position relative to the player ship texture
            laser_origin.x = static_cast<int>(player.position.x + (0.4 * player.position.width));
            laser_origin.y = static_cast<int>(round(player.position.y + (player.position.height / 2.5)));

            player_lasers.emplace_back(Laser(laser_origin, mouse_pos, player.damage, player.angle));
            player.time_last_shot_ms = current_time;
        }
    }

	{ // Update angle
        auto player_center_x = player.position.x + (player.position.width / 2);
        auto player_center_y = player.position.y + (player.position.height / 2);
        auto angle_in_radians = atan2(player_center_y - mouse_pos.y, player_center_x - mouse_pos.x);
        auto angle_in_degrees = angle_in_radians * 180 / M_PI;

        player.angle = static_cast<int>(angle_in_degrees + 180) % 360;
    }
}

void PlayMode::UpdateLasers()
{
    for (auto& laser : player_lasers)
    {
        laser.position.x += laser.direction[0] * laser.speed;
        laser.position.y += laser.direction[1] * laser.speed;
        laser.hitbox.x = round(laser.position.x);
        laser.hitbox.y = round(laser.position.y);
    }
}

void PlayMode::UpdateEnemies()
{
    for (auto& asteroid : asteroids)
    {
        asteroid.position.x += asteroid.direction[0] * asteroid.speed;
        asteroid.position.y += asteroid.direction[1] * asteroid.speed;
        asteroid.hitbox.x = round(asteroid.position.x);
        asteroid.hitbox.y = round(asteroid.position.y);
        asteroid.angle += 1.0;
    }

    for (auto& blaster : blasters)
    {
        blaster.position.x += blaster.direction[0] * blaster.speed;
        blaster.position.y += blaster.direction[1] * blaster.speed;
        blaster.hitbox.x = round(blaster.position.x);
        blaster.hitbox.y = round(blaster.position.y);
    }

    for (auto& drone : drones)
    {
        drone.position.x += drone.direction[0] * drone.speed;
        drone.position.y += drone.direction[1] * drone.speed;
        drone.hitbox.x = round(drone.position.x);
        drone.hitbox.y = round(drone.position.y);
    }
}


void PlayMode::ResolveCollisions(GameState* state)
{
    for (const auto& asteroid : asteroids)
    {
        if (asteroid.hitbox.Collides(player.hitbox))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }
    }

    for (const auto& blaster : blasters)
    {
        if (blaster.hitbox.Collides(player.hitbox))
        {
            state->player_alive = false;
            state->current_mode = Mode::QUIT;
        }
    }

    for (auto& laser : player_lasers)
    {
        for (auto& asteroid : asteroids)
        {
            if (laser.hitbox.Collides(asteroid.hitbox))
            {
                laser.health = 0;
                asteroid.health -= laser.damage;

                if (asteroid.health <= 0)
                {
                    state->player_score += enemy_values.asteroid;
                }
            }
        }

        for (auto& blaster : blasters)
        {
            if (laser.hitbox.Collides(blaster.hitbox))
            {
                laser.health = 0;
                blaster.health -= laser.damage;

                if (blaster.health <= 0)
                {
                    state->player_score += enemy_values.blaster;
                }
            }
        }
    }
}

void PlayMode::Render(Renderer* renderer, const GameState& state)
{
    renderer->DrawBackground();

    for (const auto& laser : player_lasers)
    {
        renderer->DrawPlayerLaser(laser.position, laser.angle);
    }

    for (const auto& asteroid : asteroids)
    {
        renderer->DrawAsteroid(asteroid.position, asteroid.angle);
    }

    for (const auto& blaster : blasters)
    {
        renderer->DrawBlaster(blaster.position, blaster.angle);
    }

    for (const auto& drone : drones)
    {
        renderer->DrawDrone(drone.position, drone.angle);
    }

    renderer->DrawPlayer(player.position, player.angle);
}


bool OutsideScreen(const Rectangle& rect)
{
    return (rect.x + rect.width <= 0 ||
            rect.x >= SCREEN_WIDTH ||
            rect.y + rect.height <= 0 ||
            rect.y >= SCREEN_HEIGHT);
}

void PlayMode::RemoveDeadEntities()
{
    asteroids.erase(std::remove_if(
                        begin(asteroids),
                        end(asteroids),
                        [](auto& ast) {
                            return ast.health <= 0.0 || OutsideScreen(ast.position);
                        }),
                    end(asteroids));

    blasters.erase(std::remove_if(
                       begin(blasters),
                       end(blasters),
                       [](auto& blaster) {
                           return blaster.health <= 0.0 || OutsideScreen(blaster.position);
                       }),
                   end(blasters));


    player_lasers.erase(std::remove_if(
                            begin(player_lasers),
                            end(player_lasers),
                            [](auto& laser) {
                                return laser.health <= 0.0 || OutsideScreen(laser.position);
                            }),
                        end(player_lasers));
}


Point PlayMode::GenerateSpawnPosition(const SpawnInfo& info)
{
    static std::random_device random;

    enum Side {
        LEFT   = 0,
        RIGHT  = 1,
        TOP    = 2,
        BOTTOM = 3
    };

    std::uniform_int_distribution<int> spawn_side(0, 3);
    std::uniform_int_distribution<int> spawn_range_x(0 - info.width, SCREEN_WIDTH);
    std::uniform_int_distribution<int> spawn_range_y(0 - info.height, SCREEN_HEIGHT);

    Side side = static_cast<Side>(spawn_side(random));
    Point spawn_pos;

    switch (side) {
    case LEFT:
    {
        spawn_pos.x = 0 - info.width;
        spawn_pos.y = spawn_range_y(random);
        break;
    }
    case RIGHT:
    {
        spawn_pos.x = SCREEN_WIDTH;
        spawn_pos.y = spawn_range_y(random);
        break;
    }
    case BOTTOM:
    {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = SCREEN_HEIGHT;
        break;
    }
    case TOP:
    {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = 0 - info.height;
        break;
    }
    };

    return spawn_pos;
}


void PlayMode::SpawnEnemies()
{
    auto current_time = SDL_GetTicks();

    { // Asteroids
        auto time_since_last_spawn = current_time - spawn_info_asteroid.time_last_spawned;

        if (time_since_last_spawn >= spawn_info_asteroid.delay_ms)
        {
            for (u32 i = 0; i < spawn_info_asteroid.amount; ++i)
            {
                Point spawn_pos = GenerateSpawnPosition(spawn_info_asteroid);
                asteroids.emplace_back<Asteroid>(spawn_pos);
            }

            spawn_info_asteroid.time_last_spawned = current_time;
        }
    }

    { // Blasters
        auto time_since_last_spawn = current_time - spawn_info_blaster.time_last_spawned;

        if (time_since_last_spawn >= spawn_info_blaster.delay_ms)
        {
            for (u32 i = 0; i < spawn_info_blaster.amount; ++i)
            {
                Point spawn_pos = GenerateSpawnPosition(spawn_info_blaster);
                blasters.emplace_back<Blaster>(spawn_pos);
            }

            spawn_info_blaster.time_last_spawned = current_time;
        }
    }

    { // Drones
        auto time_since_last_spawn = current_time - spawn_info_drone.time_last_spawned;

        if (time_since_last_spawn >= spawn_info_drone.delay_ms)
        {
            for (u32 i = 0; i < spawn_info_drone.amount; ++i)
            {
                Point spawn_pos = GenerateSpawnPosition(spawn_info_drone);
                drones.emplace_back<Drone>(spawn_pos);
            }

            spawn_info_drone.time_last_spawned = current_time;
        }
    }
}
