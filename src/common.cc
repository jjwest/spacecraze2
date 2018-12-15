#include "common.h"

int g_screen_width = 1920;
int g_screen_height = 1024;

bool Rectangle::intersects(const Rectangle &other) const {
    return (x <= other.x + other.width && x + width >= other.x &&
            y <= other.y + other.height && y + height >= other.y);
}

bool Rectangle::contains(Point point) const {
    return (point.x >= x && point.x <= x + width && point.y >= y &&
            point.y <= y + width);
}
