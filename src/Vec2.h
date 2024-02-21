#pragma once
#include <math.h>
// Vec2 class
// - constructor
// - add
// - dist
// - scale
// - +
// - ==
// - chaining

class Vec2
{
public:
    float x = 0;
    float y = 0;

    // Default constructor
    Vec2() {}

    // Constructor that takes in x and y
    Vec2(float xin, float yin)
        : x(xin), y(yin) { }


    Vec2 operator+(Vec2 v)
    {
        return Vec2(x + v.x, y + v.y);
    }

    // Scales vector
    void scale(float s)

    {
        x = s;
        y = s;
    }

    // Subtract
    Vec2 operator - (Vec2  v)
    {
        return Vec2(x - v.x, y - v.y);
    }

    // Length of a vector
    float length()
    {
        return sqrtf((x * x) + (y * y));
    }

    // Distance between two Vec2s
    float dist(Vec2 v)
    {
        return (v - *this).length();
    }
    // Scales vector and returns a new vector
    Vec2 operator*(float s)
    {
        return Vec2(x * s, y * s);
    }
};

