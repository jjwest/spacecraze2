#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include "renderer.h"

enum class Mode
{
    PLAY,
    QUIT,
};

struct GameState
{
    bool running = true;
    int player_score = 0;
    Mode current_mode = Mode::PLAY;
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
