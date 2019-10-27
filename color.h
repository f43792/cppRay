#ifndef __color_h__
#define __color_h__

#include <cmath>

struct Color{
    float r;
    float g;
    float b;

    void clamp(float min = 0.0f, float max = 255.0f) {
        r = std::max(min, std::min(max, r));
        g = std::max(min, std::min(max, g));
        b = std::max(min, std::min(max, b));
    }
};

#endif