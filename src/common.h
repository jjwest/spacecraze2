#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdio.h>
#include <random>
#include <stdarg.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;


extern int g_screen_width;
extern int g_screen_height;


inline void Error(const char* format, ...)
{
    fprintf(stderr, "ERROR :: ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

inline void FatalError(const char* format, ...)
{
    Error(format);
    exit(1);
}


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

inline std::pair<float, float> CalculateMovementDeltas(Point origin, Point destination)
{
    float dx = destination.x - origin.x;
    float dy = destination.y - origin.y;
    float largest = std::max( std::abs(dx), std::abs(dy) );
    float dx_per_frame = dx / largest;
    float dy_per_frame = dy / largest;

    return { dx_per_frame, dy_per_frame };
}



#endif // _COMMON_H_
