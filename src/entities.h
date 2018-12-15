#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "common.h"
#include "mesh.h"
#include "powerup.h"

struct Laser;

struct Player {
    bool has_singularity_weapon = true;
    float damage = 5.0;
    int angle = 0;
    float speed = 8.0;
    u32 shoot_cooldown_ms = 50;
    u32 time_last_shot = 0;
    Rectangle position = {(float)(g_screen_width / 2) - 25,
                          (float)(g_screen_height / 2) - 19, 50.0, 38.0};

    void update(std::vector<Laser> *player_lasers,
                const ActivatedPowerups &powerups);
};

struct Laser {
    int angle = 0;
    float health = 1.0;
    float speed = 8.0;
    float dx = 0.0;
    float dy = 0.0;
    float damage = 0.0;
    Rectangle position = {400.0, 400.0, 10.0, 20.0};

    Laser(Point origin, Point destination, float damage, int angle);

    void update();
};

struct Asteroid {
    Rectangle position = {0.0, 0.0, 75.0, 75.0};
    float health = 500.0;
    int angle = 0;
    float speed = 2.0;
    float dx = 0.0;
    float dy = 0.0;
    int score_value = 50;

    void update();
};

struct Blaster {
    Rectangle position = {0.0, 0.0, 75.0, 75.0};
    float health = 50.0;
    int angle = 0;
    float speed = 3.0;
    int score_value = 25;
    u32 shoot_cooldown_ms = 500;
    u32 time_last_shot = 0;
    Point destination;

    void update(std::vector<Laser> *lasers, Point player_pos);

private:
    void update_movement_direction();
};

struct Drone {
    Rectangle position = {0.0, 0.0, 50.0, 50.0};
    float health = 10.0;
    int angle = 0;
    float speed = 3.0;
    int score_value = 5;

    void update(Point destination);
};

#endif // _ENTITIES_H_
