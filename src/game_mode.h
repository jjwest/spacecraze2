#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

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
    virtual void render(GameState* state) const = 0;
};


#endif // _GAMEMODE_H_
