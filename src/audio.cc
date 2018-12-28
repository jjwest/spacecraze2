#include "audio.h"

#include <stdio.h>

Audio::~Audio() {
    for (auto &pair : sound_effects) {
        Mix_FreeChunk(pair.second);
    }

    for (auto &pair : music) {
        Mix_FreeMusic(pair.second);
    }
}

void Audio::load_sound_effect(const char *name, const char *file) {
    Mix_Chunk *sound_effect = Mix_LoadWAV(file);
    if (!sound_effect) {
        error("Could not load sound effect '%s'", file);
        return;
    }
    sound_effects.insert({name, sound_effect});
}

void Audio::load_music(const char *name, const char *file) {
    Mix_Music *m = Mix_LoadMUS(file);
    if (!m) {
        error("Could not load music '%s'", file);
        return;
    }
    music.insert({name, m});
}

void Audio::play_music(const char *name) {
    auto it = music.find(name);
    if (it == end(music)) {
        error("Tried playing non-existing music '%s'", name);
        return;
    }
    Mix_PlayMusic(it->second, -1);
}

void Audio::play_sound_effect(const char *name) {
    auto it = sound_effects.find(name);
    if (it == end(sound_effects)) {
        error("Tried playing non-existing sound effect '%s'", name);
        return;
    }
    Mix_PlayChannel(-1, it->second, 1);
}
