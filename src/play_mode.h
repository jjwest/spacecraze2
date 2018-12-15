#ifndef _PLAY_MODE_H_
#define _PLAY_MODE_H_

#include "common.h"
#include "entities.h"
#include "font.h"
#include "game_mode.h"
#include "mesh.h"
#include "powerup.h"

#include <SDL2/SDL.h>

#include <vector>

struct SpawnInfo {
    u32 amount;
    u32 cooldown_ms;
    u32 time_last_spawned = SDL_GetTicks();
};

enum struct Side {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

struct PlayMode : GameMode {
    PlayMode();
    virtual void handle_events(GameState *state) override;
    virtual void update(GameState *state) override;
    virtual void render(Renderer *renderer, const GameState &state) override;

    SDL_Event events;
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<Blaster> blasters;
    std::vector<Drone> drones;
    std::vector<Laser> player_lasers;
    std::vector<Laser> blaster_lasers;
    std::vector<Powerup> spawned_powerups;

    ActivatedPowerups activated_powerups;

    SpawnInfo spawn_info_asteroid = {5, 5000};
    SpawnInfo spawn_info_blaster = {1, 8000};
    SpawnInfo spawn_info_drone = {5, 1500};
    SpawnInfo spawn_info_powerup = {1, 4000};

    Texture texture_player{"../sprites/playership.png"};
    Texture texture_asteroid{"../sprites/meteor.png"};
    Texture texture_blaster{"../sprites/blaster.png"};
    Texture texture_drone{"../sprites/drone.png"};
    Texture texture_player_laser{"../sprites/playerlaser.png"};
    Texture texture_enemy_laser{"../sprites/enemylaser.png"};
    Texture texture_background{"../sprites/space.png"};
    Texture texture_double_damage{"../sprites/double_damage.png"};

    Shader laser_shader{"../shaders/laser.vert", "../shaders/laser.frag"};
    FontAnimation score_font_animation{400, 10000, glm::vec3(0.0, 1.0, 0.0)};

    std::random_device random;
    std::uniform_int_distribution<int> range_x{0, g_screen_width};
    std::uniform_int_distribution<int> range_y{0, g_screen_height};

private:
    void update_score(GameState *state, int new_score);
    void update_player();
    void update_lasers();
    void update_entities(GameState *state);
    void spawn_powerups();
    void spawn_enemies();
    void resolve_collisions(GameState *state);
    void remove_dead_entities();
    Rectangle generate_enemy_spawn_position(float width, float height);
    Rectangle generate_powerup_spawn_position(float width, float height);
    void spawn_asteroids();
    void spawn_blasters();
    void spawn_drones();
    Point generate_point_on_opposite_side(Point origin);
};

#endif // _PLAY_MODE_H_
