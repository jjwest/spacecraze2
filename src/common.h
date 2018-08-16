#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <random>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;


extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;

    bool Intersects(const Rectangle& other) const
    {
        return (x <= other.x + other.width &&
                x + width >= other.x &&
                y <= other.y + other.height &&
                y + height >= other.y);
    }
};

struct Point
{
    int x;
    int y;

    Point() = default;
    Point(const Rectangle& rect)
        : x{static_cast<int>(rect.x + rect.width / 2)},
          y{static_cast<int>(rect.y + rect.height / 2)} {}
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




#endif // _COMMON_H_