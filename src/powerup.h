#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "common.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <vector>

enum struct PowerupKind
{
    DOUBLE_DAMAGE,
    COUNT,  // This is just to help determine number of kinds
};

struct Powerup
{
    PowerupKind kind;
    u32 duration_ms;
    u32 time_activated = SDL_GetTicks();
    Rectangle position = {0.0, 0.0, 30.0, 30.0};
    int angle = 0;
    bool idle_on_map = true;
};

struct ActivatedPowerups
{
    std::vector<Powerup> powerups;

    bool Contains(PowerupKind kind) const
    {
        for (const auto& powerup : powerups)
        {
            if (powerup.kind == kind)
            {
                return true;
            }
        }

        return false;
    }

    void Update()
    {
        auto current_time = SDL_GetTicks();
        powerups.erase(
            std::remove_if(begin(powerups), end(powerups),
                           [current_time] (const Powerup& p)
                           { return current_time - p.time_activated >= p.duration_ms;}),
            end(powerups));
    }
};


#endif // _POWERUP_H_
