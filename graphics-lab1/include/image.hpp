#pragma once

#include <GL/freeglut.h>
#include <string>

using Point = std::pair<float, float>;

class Image
{
public:
    Image(const std::string &name, const Point &size, const Point &pos, float rotation = 0, const Point &origin = {0, 0}, const Point &scale = {1, 1});
    void draw() const;

    Point pos, origin, scale;
    float rotation;

private:
    GLuint texture;
    std::pair<float, float> size;
};