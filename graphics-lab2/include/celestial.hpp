#pragma once

#include <GL/freeglut.h>
#include <string>
#include <vector>

using Point = std::pair<float, float>;

class Celestial
{
public:
    Celestial(float radius, unsigned int color, const Point &pos, float rotation);
    Celestial(const std::string &name, const Point &size, float radius, const Point &pos);
    ~Celestial() = default;

    inline double getRotation() { return rotation; }

    void update();
    void drag(const Point &addPos);
    void draw() const;

private:
    double radius, rotation;
    Point pos, addPos, cntr;
    unsigned int color;
    std::vector<Point> verts;
};