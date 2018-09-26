#include "common.h"

int g_screen_width = 1920;
int g_screen_height = 1024;

bool Rectangle::Intersects(const Rectangle& other) const
{
    return (x <= other.x + other.width &&
            x + width >= other.x &&
            y <= other.y + other.height &&
            y + height >= other.y);

}

bool Rectangle::Contains(Point point) const
{
    return (point.x >= x && point.x <= x + width &&
            point.y >= y && point.y <= y + width);
}
