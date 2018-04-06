#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "aabb.h"
#include "platform.h"
#include "mesh.h"

struct Player
{
    AABB hitbox = {600, 300, 50, 40};
    float health = 20.0;
    float health_max = 20.0;
    float damage = 5.0;
    float angle = 0.0;
    float speed = 8.0;
    u32 shoot_cooldown_ms = 50;
    u32 time_last_shot_ms = 0;
    Rectangle position = {300.0, 300.0, 50.0, 38.0};
};

struct Asteroid
{
    Rectangle position = {400.0, 400.0, 75.0, 75.0};
    AABB hitbox = {300, 300, 50, 50};
    float health = 20.0;
    float health_max = 20.0;
    float angle = 0.0;
    float speed = 2.0;
    float direction[2] = {0.0, 0.0};

    Asteroid(Point spawn_pos);
};

struct Blaster
{
    Rectangle position = {400.0, 400.0, 75.0, 75.0};
    AABB hitbox = {300, 300, 50, 50};
    float health = 50.0;
    float health_max = 50.0;
    float angle = 0.0;
    float speed = 3.0;
    float direction[2] = {0.0, 0.0};

    Blaster(Point spawn_pos);
};

struct Drone
{
    Rectangle position = {400.0, 400.0, 75.0, 75.0};
    AABB hitbox = {300, 300, 50, 50};
    float health = 50.0;
    float health_max = 50.0;
    float angle = 0.0;
    float speed = 3.0;
    float direction[2] = {0.0, 0.0};

    Drone(Point spawn_pos);
};


struct Laser
{
    AABB hitbox = {300, 300, 50, 50};
    float angle = 0.0;
    float health = 1.0;
    float speed = 8.0;
    float direction[2] = {0.0, 0.0};
    float damage = 0.0;
    Rectangle position = {400.0, 400.0, 10.0, 20.0};

    Laser(Point origin, Point destination, float damage, float angle);
};

#endif // _ENTITIES_H_
