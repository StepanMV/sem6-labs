#include "image.hpp"
#include <iostream>
#include "light.hpp"

Image::Image(const std::string &name, const Point &size, const Point &pos, float rotation, const Point &origin, const Point &scale)
    : pos(pos), size(size), rotation(rotation), origin(origin), scale(scale)
{
    unsigned char *data = (unsigned char *)malloc(size.first * size.second * 4);
    FILE *file = fopen(name.c_str(), "rb");
    if (!file)
    {
        std::cout << "[ERROR] Can't load texture \"" + name + "\"\n";
        return;
    }
    fread(data, size.first * size.second * 4, 1, file);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.first, size.second, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
}

void Image::draw() const
{
    unsigned int color = Light::getLightColor();
    float red = (color & 0xFF0000) / 256.f / 256.f / 256.;
    float green = (color & 0x00FF00) / 256.f / 256.f;
    float blue = (color & 0x0000FF) / 256.f;
    // std::cout << red << " " << green << " " << blue << "\n";
    glColor4f(red, green, blue, 1.0f);
    glLoadIdentity();
    glScalef(scale.first, scale.second, 1);
    glTranslatef(pos.first / 640, pos.second / 360, 0);
    glScalef(1.0f / 16.0f, 1.0f / 9.0f, 1);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(16.0f, 9.0f, 1);
    glTranslatef(-origin.first / 640, -origin.second / 360, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f(pos.first / 640, pos.second / 360);
        glTexCoord2f(1, 1);
        glVertex2f((pos.first + size.first) / 640, pos.second / 360);
        glTexCoord2f(1, 0);
        glVertex2f((pos.first + size.first) / 640, (pos.second + size.second) / 360);
        glTexCoord2f(0, 0);
        glVertex2f(pos.first / 640, (pos.second + size.second) / 360);
    glEnd();
}
