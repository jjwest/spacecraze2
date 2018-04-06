#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include "renderer.h"
#include "platform.h"

struct GameMode
{
    GameMode() {};
    virtual ~GameMode() {};
    virtual void HandleEvents(GameState* state) = 0;
    virtual void Update(GameState* state) = 0;
    virtual void Render(Renderer* renderer, const GameState& state) = 0;
};


#endif // _GAMEMODE_H_
