#include "entities.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <math.h>

int AngleInDegrees(Point from, Point to)
{
    float angle_in_radians = atan2(to.y - from.y, to.x - from.x);
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

        bool can_move_down = position.y + position.height + speed <= g_screen_height;
        if (key_pressed[SDL_SCANCODE_S] && can_move_down)
        {
            position.y += speed;
        }

        bool can_move_left = position.x - speed >= 0;
        if (key_pressed[SDL_SCANCODE_A] && can_move_left)
        {
            position.x -= speed;
        }

        bool can_move_right = position.x + position.width + speed <= g_screen_width;
        if (key_pressed[SDL_SCANCODE_D] && can_move_right)
        {
            position.x += speed;
        }
    }

    Point mouse_position;

    { // Shoot
        bool left_mouse_button_pressed = SDL_GetMouseState(&mouse_position.x, &mouse_position.y) & SDL_BUTTON(SDL_BUTTON_LEFT);
        auto current_time = SDL_GetTicks();

        if (left_mouse_button_pressed &&
            (current_time - time_last_shot > shoot_cooldown_ms))
        {
            Point origin;
            // We hack the positions a bit to ensure that the laser originates from a good
            // position relative to the player ship texture
            origin.x = static_cast<int>(position.x + (0.4 * position.width));
            origin.y = static_cast<int>(round(position.y + (position.height / 2.5)));

            Laser laser(origin, mouse_position, damage, angle);
            laser.speed = 12.0;
            player_lasers->push_back(laser);

            time_last_shot = current_time;
        }
    }

    angle = AngleInDegrees(position, mouse_position);
}

void Asteroid::Update()
{
    position.x += dx * speed;
    position.y += dy * speed;
    angle++;
}



void Drone::Update(Point destination)
{
    auto [dx, dy] = CalculateMovementDeltas(position, destination);
    position.x += dx * speed;
    position.y += dy * speed;
    angle = AngleInDegrees(position, destination);
}


bool AtEdgeOfScreen(const Rectangle& rect)
{
    return (rect.x <= 0 ||
            rect.x + rect.width >= g_screen_width ||
            rect.y <= 0 ||
            rect.y + rect.height >= g_screen_height);
}

void Blaster::Update(std::vector<Laser>* lasers, Point player_pos)
{
    if (AtEdgeOfScreen(position))
    {
        UpdateMovementDirection();
    }

    { // Move
        auto [dx, dy] = CalculateMovementDeltas(position, destination);
        position.x += dx * speed;
        position.y += dy * speed;
    }

    angle = AngleInDegrees(position, player_pos);

    { // Shoot
        auto current_time = SDL_GetTicks();

        if (current_time - time_last_shot > shoot_cooldown_ms)
        {
            Point origin(position);
            auto angle = AngleInDegrees(origin, player_pos);

            lasers->push_back( Laser(origin, player_pos, 5.0, angle) );
            time_last_shot = current_time;
        }
    }
}

void Blaster::UpdateMovementDirection()
{
    static std::random_device random;
    static std::uniform_int_distribution<int> range_x(0, g_screen_width);
    static std::uniform_int_distribution<int> range_y(0, g_screen_height);


    if (position.x <= 0)
    {
        destination.x = g_screen_width;
        destination.y = range_y(random);
    }
    else if (position.x + position.width >= g_screen_width)
    {
        destination.x = 0;
        destination.y = range_y(random);
    }
    else if (position.y <= 0)
    {
        destination.x = range_x(random);
        destination.y = g_screen_height;
    }
    else if (position.y + position.height >= g_screen_height)
    {
        destination.x = range_x(random);
        destination.y = 0;
    }
}


Laser::Laser(Point origin, Point destination, float damage, int angle)
    : angle{angle}, damage{damage}
{
    position.x = origin.x;
    position.y = origin.y;

    auto [dx, dy] = CalculateMovementDeltas(origin, destination);
    this->dx = dx;
    this->dy = dy;
}

void Laser::Update()
{
    position.x += dx * speed;
    position.y += dy * speed;
}
