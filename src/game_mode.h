#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include "renderer.h"

struct GameState
{
    bool running = true;
};

struct GameMode
{
    GameMode() {};
    virtual ~GameMode() {};
    virtual void handleEvents(GameState* state) = 0;
    virtual void update(GameState* state) = 0;
    virtual void render(Renderer* renderer, const GameState& state) = 0;
};


#endif // _GAMEMODE_H_
