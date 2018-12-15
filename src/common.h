#ifndef _COMMON_H_
#define _COMMON_H_

#include <random>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

extern int g_screen_width;
extern int g_screen_height;

inline void _error(const char *file, int line, const char *format, ...) {
    fprintf(stderr, "ERROR %s:%d:\n      ", file, line);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

inline void _fatal_error(const char *file, int line, const char *format, ...) {
    _error(file, line, format);
    exit(1);
}

#define ERROR(...) _error(__FILE__, __LINE__, __VA_ARGS__)
#define FATAL_ERROR(...) _fatal_error(__FILE__, __LINE__, __VA_ARGS__)

struct Point;

struct Rectangle {
    float x;
    float y;
    float width;
    float height;

    bool intersects(const Rectangle &other) const;
    bool contains(Point point) const;
};

struct Point {
    int x;
    int y;

    Point() = default;
    Point(const Rectangle &rect)
        : x{static_cast<int>(rect.x + rect.width / 2)}, y{static_cast<int>(
            rect.y +
            rect.height / 2)} {}
};

enum struct Mode {
    MENU,
    PLAY,
    QUIT,
};

struct GameState {
    bool player_alive = true;
    int player_score = 0;
    Mode current_mode = Mode::PLAY;
};

inline std::pair<float, float> calculate_movement_deltas(Point origin,
                                                         Point destination) {
    float dx = destination.x - origin.x;
    float dy = destination.y - origin.y;
    float largest = std::max(std::abs(dx), std::abs(dy));
    float dx_per_frame = dx / largest;
    float dy_per_frame = dy / largest;

    return {dx_per_frame, dy_per_frame};
}

#endif // _COMMON_H_
