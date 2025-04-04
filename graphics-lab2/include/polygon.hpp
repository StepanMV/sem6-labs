#pragma once

#include <GL/freeglut.h>
#include <string>
#include <vector>

using Point = std::pair<float, float>;

class MyPolygon
{
public:
    MyPolygon(const std::vector<GLfloat> &verts, const Point &pos, unsigned int color = 0xFFFFFF, float rotation = 0, const Point &origin = {0, 0}, const Point &scale = {1, 1}, bool light = true);
    ~MyPolygon() = default;
    void draw() const;

    Point pos, origin, scale;
    float rotation;
    unsigned int color;
    bool light;

private:
    std::vector<GLfloat> verts;
};