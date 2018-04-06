#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


struct Point
{
    int x;
    int y;
};

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
};

enum struct Mode
{
    PLAY,
    QUIT,
};

struct GameState
{
    bool player_alive = true;
    int player_score = 0;
    Mode current_mode = Mode::PLAY;
};


#endif // _PLATFORM_H_
