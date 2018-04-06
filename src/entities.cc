#include "entities.h"

#include <algorithm>
#include <math.h>

Asteroid::Asteroid(Point spawn_pos)
    : position{(float)spawn_pos.x, (float)spawn_pos.y, 75.0, 75.0},
      hitbox{spawn_pos.x, spawn_pos.y, (u32)position.width, (u32)position.height},
      direction{1.0, 1.0}
{}

Blaster::Blaster(Point spawn_pos)
    : position{(float)spawn_pos.x, (float)spawn_pos.y, 75.0, 75.0},
      hitbox{spawn_pos.x, spawn_pos.y, (u32)position.width, (u32)position.height}
{}

Drone::Drone(Point spawn_pos)
    : position{(float)spawn_pos.x, (float)spawn_pos.y, 75.0, 75.0},
      hitbox{spawn_pos.x, spawn_pos.y, (u32)position.width, (u32)position.height},
      direction{1.0, 1.0}
{}

Laser::Laser(Point origin, Point destination, float damage, float angle)
{
    position.x = origin.x;
    position.y = origin.y;
    this->angle = angle;
    this->damage = damage;

    float delta_x = destination.x - origin.x;
    float delta_y = destination.y - origin.y;
    float largest = std::max(std::abs(delta_x), std::abs(delta_y));

    direction[0] = delta_x / largest;
    direction[1] = delta_y / largest;
}
