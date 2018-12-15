#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include "common.h"
#include "renderer.h"

struct GameMode {
    GameMode(){};
    virtual ~GameMode(){};
    virtual void handle_events(GameState *state) = 0;
    virtual void update(GameState *state) = 0;
    virtual void render(Renderer *renderer, const GameState &state) = 0;
};

#endif // _GAMEMODE_H_
