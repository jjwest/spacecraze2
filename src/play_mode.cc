#include "play_mode.h"

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

void PlayMode::render(GameState* state) const
{
    assert(state);

}
