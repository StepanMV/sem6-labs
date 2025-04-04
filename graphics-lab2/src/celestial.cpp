#include "celestial.hpp"
#include <cmath>
#include <iostream>
#include <GL/gl.h>

#define sign(x) (x >= 0 ? 1 : -1)
#define pi 3.14159265359

Celestial::Celestial(float radius, unsigned int color, const Point &pos, float rotation)
    : radius(radius), color(color), pos(pos), rotation(rotation), cntr(Point(0, pos.second - radius))
{
    for (size_t i = 0; i <= 360; ++i)
        verts.push_back({32 * std::cos(i * 3.14 / 180), 32 * std::sin(i * 3.14 / 180)});
}

Celestial::Celestial(const std::string &name, const Point &size, float radius, const Point &pos)
{
}

void Celestial::update()
{
    pos.first = cntr.first + radius * std::cos(rotation * pi / 180) + addPos.first;
    pos.second = cntr.second + radius * std::sin(rotation * pi / 180) + addPos.second;
    rotation -= 0.01;
    rotation = rotation <= -35 ? rotation + 180 : rotation;
}

void Celestial::drag(const Point &addPos)
{
    this->addPos = addPos;
}

void Celestial::draw() const
{
    glColor3f(((color >> 16) & 0xFF) / 255.f, ((color >> 8) & 0xFF) / 255.f, (color & 0xFF) / 255.f);
    glLoadIdentity();
    glTranslatef(pos.first / 640, pos.second / 360, 0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (const auto &vert : verts)
        glVertex2f(vert.first / 640, vert.second / 360);
    glEnd();
}
