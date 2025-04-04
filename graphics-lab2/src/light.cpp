#include "light.hpp"
#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <algorithm>

unsigned char *Light::lightData;
double Light::time = 0;
Point Light::size;

void Light::load(const std::string &name, const Point &size)
{
    Light::size = size;
    lightData = (unsigned char *)malloc(size.first * size.second * 4);
    FILE *file = fopen(name.c_str(), "rb");
    if (!file)
    {
        std::cout << "[ERROR] Can't load texture \"" + name + "\"\n";
        return;
    }
    fread(lightData, size.first * size.second * 4, 1, file);
}

unsigned int Light::getLightColor()
{
    int x = std::clamp(static_cast<int>(time * (size.first - 1)), 0, (int)size.first - 1);
    int y = size.second / 2;

    int channels = 4;
    int sumR = 0, sumG = 0, sumB = 0;
    int count = 0;

    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = std::clamp(x + dx, 0, (int)size.first - 1);
            int ny = std::clamp(y + dy, 0, (int)size.second - 1);
            int index = (ny * size.first + nx) * channels;

            sumR += lightData[index];
            sumG += lightData[index + 1];
            sumB += lightData[index + 2];
            count++;
        }
    }
    // std::cout << sumR / count << " " << sumG / count << " " << sumB / count << "\n";
    return (sumR / count << 16) | (sumG / count << 8) | (sumB / count);
}
