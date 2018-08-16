#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "common.h"
#include "mesh.h"

struct Laser;

struct Player
{
    float health = 20.0;
    float health_max = 20.0;
    float damage = 5.0;
    int angle = 0;
    float speed = 8.0;
    u32 shoot_cooldown_ms = 50;
    u32 time_last_shot_ms = 0;
    Rectangle position = {300.0, 300.0, 50.0, 38.0};

    void Update(std::vector<Laser>* player_lasers);
};

struct Laser
{
    int angle = 0;
    float health = 1.0;
    float speed = 8.0;
    float dx = 0.0;
    float dy = 0.0;
    float damage = 0.0;
    Rectangle position = {400.0, 400.0, 10.0, 20.0};

    Laser(Point origin, Point destination, float damage, int angle);

    void Update();
};

struct Asteroid
{
    Rectangle position = {0.0, 0.0, 75.0, 75.0};
    float health = 500.0;
    float health_max = 20.0;
    int angle = 0;
    float speed = 2.0;
    float dx = 0.0;
    float dy = 0.0;
    int score_value = 50;

    void Update();
};

struct Blaster
{
    Rectangle position = {0.0, 0.0, 75.0, 75.0};
    float health = 50.0;
    float health_max = 50.0;
    int angle = 0;
    float speed = 3.0;
    int score_value = 25;
    u32 shoot_cooldown_ms = 500;
    u32 time_last_shot_ms = 0;
    Point move_to;

    void Update(std::vector<Laser>* lasers, const Point& player_pos);
    void MaybeUpdateDirection();
};


struct Drone
{
    Rectangle position = {0.0, 0.0, 50.0, 50.0};
    float health = 10.0;
    float health_max = 10.0;
    int angle = 0;
    float speed = 3.0;
    float dx = 0.0;
    float dy = 0.0;
    int score_value = 5;

    void Update(const Point& destination);
};




#endif // _ENTITIES_H_
