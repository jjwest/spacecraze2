#ifndef _PLAY_MODE_H_
#define _PLAY_MODE_H_

#include "game_mode.h"

#include <SDL2/SDL.h>

struct PlayMode: public GameMode
{
    SDL_Event events;



    PlayMode();
    virtual void handleEvents(GameState* state) override;
    virtual void update(GameState* state) override;
    virtual void render(GameState* state) const override;
};

#endif // _PLAY_MODE_H_
