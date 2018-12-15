#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <SDL_mixer.h>
#include <unordered_map>

#include "common.h"

struct Audio {
    ~Audio();

    std::unordered_map<const char *, Mix_Chunk *> sound_effects;
    std::unordered_map<const char *, Mix_Music *> music;

    void load_sound_effect(const char *name, const char *file);
    void load_music(const char *name, const char *file);
    void play_music(const char *name);
    void play_sound_effect(const char *name);
};


#endif // _AUDIO_H_
