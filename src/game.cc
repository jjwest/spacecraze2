#include "game.h"

#include "common.h"
#include "play_mode.h"
#include "menu.h"

#include <glad/glad.h>
#include <SDL2/SDL_mixer.h>

#include <stdexcept>

Game::Game()
{
    InitSubsystems();
}

Game::~Game()
{
    ShutdownSubsystems();
}

void Game::Run()
{
    const u32 intended_frame_duration_ms = 16;
    GameState state;
    Renderer renderer;

    current_mode = std::make_unique<PlayMode>();

    while (state.current_mode != Mode::QUIT)
    {
        u32 start_time = SDL_GetTicks();

        current_mode->HandleEvents(&state);
        current_mode->Update(&state);
        current_mode->Render(&renderer, state);

        SDL_GL_SwapWindow(window);

        u32 time_elapsed = SDL_GetTicks() - start_time;

        if (time_elapsed < intended_frame_duration_ms)
        {
            SDL_Delay(intended_frame_duration_ms - time_elapsed);
        }
    }

    std::cout << "SCORE: " << state.player_score << '\n';
}

void Game::InitSubsystems()
{
    // INIT SDL
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
        throw std::runtime_error("Failed to initialize SDL");
    }
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        throw std::runtime_error("Failed to initialize SDL_Mixer");
    }

    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetSwapInterval(1);

    bool fullscreen = true;

    if (fullscreen)
    {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);

        g_screen_width = mode.w;
        g_screen_height = mode.h;

        window = SDL_CreateWindow(
            "SPACECRAZE",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            g_screen_width,
            g_screen_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        window = SDL_CreateWindow(
            "SPACECRAZE",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            g_screen_width,
            g_screen_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    }

    if (!window)
    {
        throw std::runtime_error("Failed to create window");
    }

    gl_context = SDL_GL_CreateContext(window);

    // INIT OPENGL
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, g_screen_width, g_screen_height);
}

void Game::ShutdownSubsystems()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    SDL_Quit();
}
