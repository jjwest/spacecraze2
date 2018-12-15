#include "play_mode.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <random>

#include <math.h>

PlayMode::PlayMode() {
    bind_shader(laser_shader.id);

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<float>(g_screen_width),
                                      static_cast<float>(g_screen_height),
                                      0.0f,
                                      -1.0f,
                                      1.0f);

    laser_shader.set_mat4("projection", projection);
}

void PlayMode::update_score(GameState *state, int new_score) {
    state->player_score = new_score;
    score_font_animation.time_started_ms = SDL_GetTicks();
}

void PlayMode::handle_events(GameState *state) {
    while (SDL_PollEvent(&events) != 0) {
        if (events.type == SDL_QUIT) {
            state->current_mode = Mode::QUIT;
        } else if (events.type == SDL_KEYDOWN &&
                   events.key.keysym.scancode == SDL_SCANCODE_Q) {
            state->current_mode = Mode::QUIT;
        }
    }
}

void PlayMode::update(GameState *state) {
    update_entities(state);
    resolve_collisions(state);
    spawn_powerups();
    spawn_enemies();
    remove_dead_entities();
}

void PlayMode::update_entities(GameState *state) {
    player.update(&player_lasers, activated_powerups);
    activated_powerups.update();

    bool right_mouse_button_pressed =
        SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    if (right_mouse_button_pressed && player.has_singularity_weapon) {
        int added_score = 0;
        for (const auto &asteroid : asteroids) {
            added_score += asteroid.score_value;
        }
        for (const auto &blaster : blasters) {
            added_score += blaster.score_value;
        }

        for (const auto &drone : drones) {
            added_score += drone.score_value;
        }

        update_score(state, state->player_score + added_score);

        asteroids.clear();
        blasters.clear();
        drones.clear();
        blaster_lasers.clear();

        player.has_singularity_weapon = false;
    }

    for (auto &asteroid : asteroids) {
        asteroid.update();
    }

    for (auto &blaster : blasters) {
        blaster.update(&blaster_lasers, player.position);
    }

    for (auto &drone : drones) {
        drone.update(player.position);
    }

    for (auto &laser : player_lasers) {
        laser.update();
    }

    for (auto &laser : blaster_lasers) {
        laser.update();
    }

    for (auto &powerup : spawned_powerups) {
        powerup.angle++;
    }
}

void PlayMode::spawn_powerups() {
    SpawnInfo *info = &spawn_info_powerup;
    u32 time_since_last_spawn = SDL_GetTicks() - info->time_last_spawned;

    if (time_since_last_spawn > info->cooldown_ms) {
        Powerup powerup;
        int powerup_count = static_cast<int>(PowerupKind::COUNT);
        std::uniform_int_distribution<int> select_powerup{0, powerup_count - 1};
        PowerupKind kind = static_cast<PowerupKind>(select_powerup(random));
        powerup.kind = kind;
        powerup.position = generate_powerup_spawn_position(powerup.position.width,
                                                           powerup.position.height);

        switch (kind) {
        case PowerupKind::DOUBLE_DAMAGE:
            powerup.duration_ms = 3000;
            break;

        default:
            ERROR("Tried to instantiate powerup of kind COUNT.");
            break;
        }

        spawned_powerups.push_back(powerup);
        info->time_last_spawned = SDL_GetTicks();
    }
}

void PlayMode::resolve_collisions(GameState *state) {
    for (const Asteroid &asteroid : asteroids) {
        if (asteroid.position.intersects(player.position)) {
            state->player_alive = false;
        }

        for (Drone &drone : drones) {
            if (asteroid.position.intersects(drone.position)) {
                drone.health = 0;
            }
        }
    }

    for (const Blaster &blaster : blasters) {
        if (blaster.position.intersects(player.position)) {
            state->player_alive = false;
        }
    }

    for (const Drone &drone : drones) {
        if (drone.position.intersects(player.position)) {
            state->player_alive = false;
        }
    }

    for (const Laser &laser : blaster_lasers) {
        if (laser.position.intersects(player.position)) {
            state->player_alive = false;
        }
    }

    for (Laser &laser : player_lasers) {
        for (Asteroid &asteroid : asteroids) {
            if (laser.position.intersects(asteroid.position)) {
                laser.health = 0;
                asteroid.health -= laser.damage;

                if (asteroid.health <= 0) {
                    update_score(state, state->player_score + asteroid.score_value);
                }
            }
        }

        for (Blaster &blaster : blasters) {
            if (laser.position.intersects(blaster.position)) {
                laser.health = 0;
                blaster.health -= laser.damage;

                if (blaster.health <= 0) {
                    update_score(state, state->player_score + blaster.score_value);
                }
            }
        }

        for (Drone &drone : drones) {
            if (laser.position.intersects(drone.position)) {
                laser.health = 0;
                drone.health -= laser.damage;

                if (drone.health <= 0) {
                    update_score(state, state->player_score + drone.score_value);
                }
            }
        }
    }

    for (Powerup &powerup : spawned_powerups) {
        if (powerup.position.intersects(player.position)) {
            powerup.time_activated = SDL_GetTicks();
            activated_powerups.powerups.push_back(powerup);
            powerup.idle_on_map = false;
        }
    }
}

void PlayMode::render(Renderer *renderer, const GameState &state) {
    renderer->draw_background(texture_background);

    bind_shader(laser_shader.id);
    laser_shader.set_bool(
        "doubleDamage", activated_powerups.contains(PowerupKind::DOUBLE_DAMAGE));

    for (const Powerup &powerup : spawned_powerups) {
        renderer->draw_rect(powerup.position, texture_double_damage, powerup.angle);
    }

    for (const Laser &laser : player_lasers) {
        renderer->draw_rect(laser.position, texture_player_laser, laser.angle,
                            &laser_shader);
    }

    for (const Asteroid &asteroid : asteroids) {
        renderer->draw_rect(asteroid.position, texture_asteroid, asteroid.angle);
    }

    for (const Blaster &blaster : blasters) {
        renderer->draw_rect(blaster.position, texture_blaster, blaster.angle);
    }

    for (const Drone &drone : drones) {
        renderer->draw_rect(drone.position, texture_drone, drone.angle);
    }

    for (const Laser &laser : blaster_lasers) {
        renderer->draw_rect(laser.position, texture_enemy_laser, laser.angle);
    }

    renderer->draw_rect(player.position, texture_player, player.angle);

    std::string score = "Score: " + std::to_string(state.player_score);
    draw_animated_text(score, g_screen_width - 300.0, 50.0, 1, glm::vec3(1),
                       score_font_animation);

    if (activated_powerups.contains(PowerupKind::DOUBLE_DAMAGE)) {
        float x = player.position.x - 60.0;
        float y = player.position.y + player.position.height + 30.0;
        draw_text("DOUBLE DAMAGE!", x, y, 0.6, glm::vec3(1.0, 1.0, 0.0));
    }
}

bool outside_screen(const Rectangle &rect) {
    return (rect.x + rect.width < 0 || rect.x > g_screen_width ||
            rect.y + rect.height < 0 || rect.y > g_screen_height);
}

void PlayMode::remove_dead_entities() {
    auto is_dead = [](auto &obj) {
                       return obj.health <= 0.0 || outside_screen(obj.position);
                   };

    asteroids.erase(std::remove_if(begin(asteroids), end(asteroids), is_dead),
                    end(asteroids));
    blasters.erase(std::remove_if(begin(blasters), end(blasters), is_dead),
                   end(blasters));
    drones.erase(std::remove_if(begin(drones), end(drones), is_dead),
                 end(drones));
    player_lasers.erase(
        std::remove_if(begin(player_lasers), end(player_lasers), is_dead),
        end(player_lasers));
    spawned_powerups.erase(
        std::remove_if(begin(spawned_powerups), end(spawned_powerups),
                       [](const Powerup &p) { return !p.idle_on_map; }),
        end(spawned_powerups));
}

Rectangle PlayMode::generate_enemy_spawn_position(float width, float height) {
    std::uniform_int_distribution<int> spawn_side(0, 3);
    std::uniform_int_distribution<int> spawn_range_x(0 - width, g_screen_width);
    std::uniform_int_distribution<int> spawn_range_y(0 - height, g_screen_height);

    Side side = static_cast<Side>(spawn_side(random));
    Rectangle spawn_pos;
    spawn_pos.width = width;
    spawn_pos.height = height;

    switch (side) {
    case Side::LEFT: {
        spawn_pos.x = 0 - width;
        spawn_pos.y = spawn_range_y(random);
    } break;
    case Side::RIGHT: {
        spawn_pos.x = g_screen_width;
        spawn_pos.y = spawn_range_y(random);
    } break;
    case Side::BOTTOM: {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = g_screen_height;
    } break;
    case Side::TOP: {
        spawn_pos.x = spawn_range_x(random);
        spawn_pos.y = 0 - height;
    } break;
    };

    return spawn_pos;
}

Rectangle PlayMode::generate_powerup_spawn_position(float width, float height) {
    std::uniform_int_distribution<int> spawn_range_x(0, g_screen_width - width);
    std::uniform_int_distribution<int> spawn_range_y(0, g_screen_height - height);

    Rectangle spawn_pos;
    spawn_pos.width = width;
    spawn_pos.height = height;
    spawn_pos.x = spawn_range_x(random);
    spawn_pos.y = spawn_range_y(random);

    return spawn_pos;
}

void PlayMode::spawn_enemies() {
    spawn_asteroids();
    spawn_blasters();
    spawn_drones();
}

Point PlayMode::generate_point_on_opposite_side(Point origin) {
    Point destination;

    if (origin.x < g_screen_width / 2) {
        destination.x = g_screen_width;
        destination.y = range_y(random);
    } else if (origin.x >= g_screen_width) {
        destination.x = 0;
        destination.y = range_y(random);
    }
    if (origin.y < g_screen_height / 2) {
        destination.y = g_screen_height;
        destination.x = range_x(random);
    } else if (origin.y >= g_screen_height) {
        destination.y = 0;
        destination.x = range_x(random);
    }

    return destination;
}

void PlayMode::spawn_asteroids() {
    SpawnInfo *spawn = &spawn_info_asteroid;
    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms) {
        for (u32 i = 0; i < spawn->amount; ++i) {
            Asteroid asteroid;
            asteroid.position = generate_enemy_spawn_position(
                asteroid.position.width, asteroid.position.height);
            Point destination = generate_point_on_opposite_side(asteroid.position);
            auto [dx, dy] = calculate_movement_deltas(asteroid.position, destination);
            asteroid.dx = dx;
            asteroid.dy = dy;

            asteroids.push_back(asteroid);
        }

        spawn->time_last_spawned = current_time;
    }
}

void PlayMode::spawn_drones() {
    SpawnInfo *spawn = &spawn_info_drone;

    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms) {
        for (u32 i = 0; i < spawn->amount; ++i) {
            Drone drone;
            drone.position = generate_enemy_spawn_position(drone.position.width,
                                                           drone.position.height);
            drones.push_back(drone);
        }

        spawn->time_last_spawned = current_time;
    }
}

void PlayMode::spawn_blasters() {
    SpawnInfo *spawn = &spawn_info_blaster;

    auto current_time = SDL_GetTicks();
    auto time_since_last_spawn = current_time - spawn->time_last_spawned;

    if (time_since_last_spawn >= spawn->cooldown_ms) {
        for (u32 i = 0; i < spawn->amount; ++i) {
            Blaster blaster;
            blaster.position = generate_enemy_spawn_position(blaster.position.width,
                                                             blaster.position.height);
            blasters.push_back(blaster);
        }

        spawn->time_last_spawned = current_time;
    }
}
