#include "game.h"

#include "constants.h"
#include "play_mode.h"

#include <glad/glad.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <stdexcept>

Game::Game()
{
    initSubsystems();
}

Game::~Game()
{
    shutdownSubsystems();
}

void Game::run()
{
    const Uint32 frame_duration_ms = 16;
    GameState state;
    Renderer renderer;

    current_mode = std::make_unique<PlayMode>(PlayMode());

    while (state.running)
    {
        Uint32 start_time = SDL_GetTicks();

        current_mode->handleEvents(&state);
        current_mode->update(&state);
        current_mode->render(&renderer, state);

        SDL_GL_SwapWindow(window);

        Uint32 time_elapsed = SDL_GetTicks() - start_time;

        if (time_elapsed < frame_duration_ms)
        {
            SDL_Delay(frame_duration_ms - time_elapsed);
        }

    }
}

void Game::initSubsystems()
{
    { // INIT SDL
        if ( SDL_Init(SDL_INIT_VIDEO) != 0 || SDL_Init(SDL_INIT_AUDIO) != 0 )
        {
            throw std::runtime_error("Failed to initialize SDL");
        }
        if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            throw std::runtime_error("Failed to initialize SDL_Mixer");
        }
        if ( TTF_Init() != 0 )
        {
            throw std::runtime_error("Failed to initialize TTF_init");
        }

        SDL_GL_LoadLibrary(NULL);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetSwapInterval(1);
    }

    window = SDL_CreateWindow(
		"SPACECRAZE",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    gl_context = SDL_GL_CreateContext(window);

    // INIT OPENGL
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::shutdownSubsystems()
{
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
