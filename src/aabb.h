#ifndef _AABB_H_
#define _AABB_H_

struct AABB
{
    int x;
    int y;
    unsigned int width;
    unsigned int height;

    bool collides(const AABB& other) const
    {
        return (static_cast<unsigned int>(x) <= other.x + other.width &&
                x + width >= static_cast<unsigned int>(other.x) &&
                static_cast<unsigned int>(y) <= other.y + other.height &&
                y + height >= static_cast<unsigned int>(other.y));
    }
};

#endif // _AABB_H_
