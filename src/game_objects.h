#ifndef _GAME_OBJECTS_H_
#define _GAME_OBJECTS_H_

#include "aabb.h"
#include "mesh.h"
#include "renderer.h"

struct Player
{
    AABB hitbox{300, 300, 50, 40};
    float health = 20.0;
    float health_max = 20.0;
    float damage = 5.0;
    float angle = 0.0;
    Rectangle position{300.0, 300.0, 50.0, 38.0};
};

struct Asteroid
{
    AABB hitbox{300, 300, 50, 50};
    float health = 20.0;
    float health_max = 20.0;
    float angle = 0.0;
    Rectangle position{400.0, 400.0, 50.0, 50.0};
};

#endif // _GAME_OBJECTS_H_
