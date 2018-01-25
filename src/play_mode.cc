#include "play_mode.h"

#include "mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>
#include <vector>


PlayMode::PlayMode()
{
}

void PlayMode::handleEvents(GameState* state)
{
    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT)
        {
            state->running = false;
            state->current_mode = Mode::QUIT;
        }
    }

}

void PlayMode::update(GameState* state)
{
    auto key_pressed = SDL_GetKeyboardState(NULL);

    bool diagonal_movement =
        (key_pressed[SDL_SCANCODE_W] || key_pressed[SDL_SCANCODE_S]) &&
        (key_pressed[SDL_SCANCODE_A] || key_pressed[SDL_SCANCODE_D]);

    float speed = diagonal_movement ? 8.5 : 10.0;

    bool can_move_up = player.position.y - speed >= 0;
    if (key_pressed[SDL_SCANCODE_W] && can_move_up)
    {
        player.position.y -= speed;
        player.hitbox.y = round(player.position.y);
    }

    bool can_move_down = player.position.y + player.hitbox.height + speed <= SCREEN_HEIGHT;
    if (key_pressed[SDL_SCANCODE_S] && can_move_down)
    {
        player.position.y += speed;
        player.hitbox.y = round(player.position.y);
    }

    bool can_move_left = player.position.x - speed >= 0;
    if (key_pressed[SDL_SCANCODE_A] && can_move_left)
    {
        player.position.x -= speed;
        player.hitbox.x = round(player.position.x);
    }

    bool can_move_right = player.position.x + player.hitbox.width + speed <= SCREEN_WIDTH;
    if (key_pressed[SDL_SCANCODE_D] && can_move_right)
    {
        player.position.x += speed;
        player.hitbox.x = round(player.position.x);
    }

    int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	auto player_center_x = player.position.x + (player.position.width / 2);
	auto player_center_y = player.position.y + (player.position.height / 2);
	auto angle_in_radians = atan2(player_center_y - mouse_y, player_center_x - mouse_x);
	auto angle_in_degrees = angle_in_radians * 180 / M_PI;

    player.angle = static_cast<int>(angle_in_degrees + 90) % 360;
}

void PlayMode::render(Renderer* renderer, const GameState& state)
{
    renderer->drawBackground();
    renderer->drawPlayer(player.position, player.angle);

    for (const Asteroid& asteroid : asteroids)
    {
        renderer->drawAsteroid(asteroid.position, asteroid.angle);
    }
}
