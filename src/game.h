#ifndef _GAME_H_
#define _GAME_H_

#include "game_mode.h"
#include "renderer.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <memory>

struct Game
{
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context;
    std::unique_ptr<GameMode> current_mode = nullptr;

    Game();
    ~Game();

    void Run();
    void InitSubsystems();
    void ShutdownSubsystems();
};



#endif // _GAME_H_
