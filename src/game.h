#ifndef _GAME_H_
#define _GAME_H_

#include "game_mode.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <memory>

struct Game {
    SDL_Window *window = nullptr;
    SDL_GLContext gl_context;
    std::unique_ptr<GameMode> current_mode = nullptr;

    Game();
    ~Game();

    void run();
    void init_subsystems();
    void shutdown_subsystems();
};

#endif // _GAME_H_
