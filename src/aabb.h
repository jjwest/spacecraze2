#ifndef _AABB_H_
#define _AABB_H_

#include "platform.h"

struct AABB
{
    int x;
    int y;
    u32 width;
    u32 height;

    bool Collides(const AABB& other) const
    {
        return (static_cast<u32>(x) <= other.x + other.width &&
                x + width >= static_cast<u32>(other.x) &&
                static_cast<u32>(y) <= other.y + other.height &&
                y + height >= static_cast<u32>(other.y));
    }
};

#endif // _AABB_H_
