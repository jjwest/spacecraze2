#ifndef _PLAY_MODE_H_
#define _PLAY_MODE_H_

#include "game_mode.h"
#include "game_objects.h"

#include <SDL2/SDL.h>

#include <vector>

struct PlayMode: public GameMode
{
    SDL_Event events;
    Player player;
    std::vector<Asteroid> asteroids{Asteroid{}, Asteroid{}};


    PlayMode();
    virtual void handleEvents(GameState* state) override;
    virtual void update(GameState* state) override;
    virtual void render(Renderer* renderer, const GameState& state) override;
};

#endif // _PLAY_MODE_H_
