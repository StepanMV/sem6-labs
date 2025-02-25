#pragma once

#include <GL/freeglut.h>
#include <string>
#include <vector>

using Point = std::pair<float, float>;

class Light
{
public:
    static void load(const std::string &name, const Point &size);
    static unsigned int getLightColor();
    static double time;

private:
    static Point size;
    static unsigned char *lightData;
};