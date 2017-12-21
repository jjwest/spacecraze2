#include "play_mode.h"

#include <glad/glad.h>

#include <cassert>

PlayMode::PlayMode() {}

void PlayMode::handleEvents(GameState* state)
{
    assert(state);

    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT)
        {
            state->running = false;
        }
    }
}

void PlayMode::update(GameState* state)
{
    assert(state);

}

void PlayMode::render(Renderer* renderer, const GameState& state)
{
    assert(state);
    renderer->draw_background();
}
