#include "entities.h"

#include "common.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <math.h>

int AngleInDegrees(const Point& from, const Point& to)
{
    float angle_in_radians = atan2(from.y - to.y, from.x - to.x);
    float angle_in_degrees = angle_in_radians * 180 / M_PI;

    return static_cast<int>(angle_in_degrees) % 360;
}

void Player::Update(std::vector<Laser>* player_lasers)
{
    { // Update movement
        auto key_pressed = SDL_GetKeyboardState(NULL);

        bool diagonal_movement =
            (key_pressed[SDL_SCANCODE_W] || key_pressed[SDL_SCANCODE_S]) &&
            (key_pressed[SDL_SCANCODE_A] || key_pressed[SDL_SCANCODE_D]);

        float speed = diagonal_movement ? this->speed * 0.75 : this->speed;

        bool can_move_up = position.y - speed >= 0;
        if (key_pressed[SDL_SCANCODE_W] && can_move_up)
        {
            position.y -= speed;
        }

        bool can_move_down = position.y + position.height + speed <= SCREEN_HEIGHT;
        if (key_pressed[SDL_SCANCODE_S] && can_move_down)
        {
            position.y += speed;
        }

        bool can_move_left = position.x - speed >= 0;
        if (key_pressed[SDL_SCANCODE_A] && can_move_left)
        {
            position.x -= speed;
        }

        bool can_move_right = position.x + position.width + speed <= SCREEN_WIDTH;
        if (key_pressed[SDL_SCANCODE_D] && can_move_right)
        {
            position.x += speed;
        }
    }

    Point mouse_pos;

    { // Shoot
        bool left_mouse_button_pressed = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y) & SDL_BUTTON(SDL_BUTTON_LEFT);
        auto current_time = SDL_GetTicks();

        if (left_mouse_button_pressed &&
            (current_time - time_last_shot_ms > shoot_cooldown_ms))
        {
            Point laser_origin;
            // We hack the positions a bit to ensure the laser originates from a good
            // position relative to the player ship texture
            laser_origin.x = static_cast<int>(position.x + (0.4 * position.width));
            laser_origin.y = static_cast<int>(round(position.y + (position.height / 2.5)));

            Laser laser(laser_origin, mouse_pos, damage, angle);
            laser.speed = 12.0;
            player_lasers->push_back(laser);

            time_last_shot_ms = current_time;
        }
    }

    angle = AngleInDegrees(mouse_pos, position);
}

void Asteroid::Update()
{
    position.x += dx * speed;
    position.y += dy * speed;
    angle += 1.0;
}


void Drone::Update(const Point& destination)
{
    float center_x = position.x + position.width / 2;
    float center_y = position.y + position.height / 2;
    float distance_x = destination.x - center_x;
    float distance_y = destination.y - center_y;
    float longest = std::max(abs(distance_x), abs(distance_y));

    float dx = distance_x / longest;
    float dy = distance_y / longest;

    position.x += dx * speed;
    position.y += dy * speed;

    angle = AngleInDegrees(position, destination);
}


void Blaster::Update(std::vector<Laser>* lasers, const Point& player_pos)
{
    MaybeUpdateDirection();

    { // Move
        float distance_x = move_to.x - position.x;
        float distance_y = move_to.y - position.y;
        float longest = std::max( std::abs(distance_x), std::abs(distance_y) );
        float dx = distance_x / longest;
        float dy = distance_y / longest;

        position.x += dx * speed;
        position.y += dy * speed;
    }

    { // Shoot
        auto current_time = SDL_GetTicks();

        if (current_time - time_last_shot_ms > shoot_cooldown_ms)
        {
            Point origin(position);
            int angle = AngleInDegrees(player_pos, origin);

            lasers->push_back(Laser(origin, player_pos, 5.0, angle));
            time_last_shot_ms = current_time;
        }
    }

    angle = AngleInDegrees(position, player_pos);
}

void Blaster::MaybeUpdateDirection()
{
    static std::random_device random;
    static std::uniform_int_distribution<int> range_x(0, SCREEN_WIDTH);
    static std::uniform_int_distribution<int> range_y(0, SCREEN_HEIGHT);


    if (position.x <= 0)
    {
        move_to.x = SCREEN_WIDTH;
        move_to.y = range_y(random);
    }
    else if (position.x + position.width >= SCREEN_WIDTH)
    {
        move_to.x = 0;
        move_to.y = range_y(random);
    }
    else if (position.y <= 0)
    {
        move_to.x = range_x(random);
        move_to.y = SCREEN_HEIGHT;
    }
    else if (position.y + position.height >= SCREEN_HEIGHT)
    {
        move_to.x = range_x(random);
        move_to.y = 0;
    }
}


Laser::Laser(Point origin, Point destination, float damage, int angle)
{
    position.x = origin.x;
    position.y = origin.y;
    this->angle = angle;
    this->damage = damage;

    float dx = destination.x - origin.x;
    float dy = destination.y - origin.y;
    float largest = std::max(std::abs(dx), std::abs(dy));

    this->dx = dx / largest;
    this->dy = dy / largest;
}

void Laser::Update()
{
    position.x += dx * speed;
    position.y += dy * speed;
}
