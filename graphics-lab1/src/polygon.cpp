#include "polygon.hpp"
#include <iostream>
#include "light.hpp"

MyPolygon::MyPolygon(const std::vector<Point> &verts, const Point &pos, unsigned int color, float rotation, const Point &origin, const Point &scale, bool light)
    : verts(verts), pos(pos), rotation(rotation), origin(origin), color(color), scale(scale), light(light) {}

void MyPolygon::draw() const
{
    unsigned int lightColor = Light::getLightColor();
    float red, green, blue;
    if (light)
    {
        red = ((color >> 24) & 0xFF) / 255.f * ((lightColor >> 16) & 0xFF) / 255.f;
        green = ((color >> 16) & 0xFF) / 255.f * ((lightColor >> 8) & 0xFF) / 255.f;
        blue = ((color >> 8) & 0xFF) / 255.f * (lightColor & 0xFF) / 255.f;
    }
    else
    {
        red = ((color >> 24) & 0xFF) / 255.f;
        green = ((color >> 16) & 0xFF) / 255.f;
        blue = ((color >> 8) & 0xFF) / 255.f;
    }
    float alpha = (color & 0xFF) / 255.f;
    glColor4f(red, green, blue, alpha);
    glLoadIdentity();
    glScalef(scale.first, scale.second, 1);
    glTranslatef(pos.first / 640, pos.second / 360, 0);
    glScalef(1.0f / 16.0f, 1.0f / 9.0f, 1);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(16.0f, 9.0f, 1);
    glTranslatef(-origin.first / 640, -origin.second / 360, 0);

    glBegin(GL_TRIANGLE_STRIP);
    for (const auto &vert : verts)
        glVertex2f(vert.first / 640, vert.second / 360);
    glEnd();
}
