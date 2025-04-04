#include <GL/freeglut.h>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include "polygon.hpp"
#include "image.hpp"
#include "celestial.hpp"
#include "light.hpp"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

// i'm lazy
std::uniform_int_distribution<int> xDist(-640, 640);
std::uniform_int_distribution<int> yDist(-360, -50);
std::uniform_int_distribution<int> subDist(-50, 50);
std::uniform_int_distribution<int> colorDist(0, 12);

unsigned int flowerColors[] = {0xFFEC00FF, 0xFF0000FF, 0x2A94DEFF, 0xA84DB8FF, 0xDADADAFF, 0xD3272FFF, 0xE3581CFF, 0xF2E6D2FF, 0xE7A2A5FF, 0xFFFFFFFF, 0x4663B3FF, 0xFFFFFFFF, 0xFFEC00FF};

std::vector<std::shared_ptr<MyPolygon>> pols;
std::vector<std::shared_ptr<Image>> bg;
std::shared_ptr<Celestial> sun, moon;

unsigned int getRainbow(float t)
{
    float hp = (int)(t * 255) % 256 / 42.5f;
    float f = hp - (int)hp;

    int q = 255 * (1 - f);
    int tCol = 255 * f;

    int r, g, b;
    switch ((int)hp)
    {
    case 0:
        r = 255;
        g = tCol;
        b = 0;
        break;
    case 1:
        r = q;
        g = 255;
        b = 0;
        break;
    case 2:
        r = 0;
        g = 255;
        b = tCol;
        break;
    case 3:
        r = 0;
        g = q;
        b = 255;
        break;
    case 4:
        r = tCol;
        g = 0;
        b = 255;
        break;
    case 5:
        r = 255;
        g = 0;
        b = q;
        break;
    default:
        r = g = b = 0;
        break;
    }

    return (r << 24) | (g << 16) | (b << 8) | 0;
}

unsigned int mixColors(unsigned int f, unsigned int s)
{
    unsigned int r1 = (f >> 16) & 0xFF;
    unsigned int g1 = (f >> 8) & 0xFF;
    unsigned int b1 = f & 0xFF;

    unsigned int r2 = (s >> 16) & 0xFF;
    unsigned int g2 = (s >> 8) & 0xFF;
    unsigned int b2 = s & 0xFF;

    unsigned int r = r1 * r2 / 255;
    unsigned int g = g1 * g2 / 255;
    unsigned int b = b1 * b2 / 255;

    return (r << 16) | (g << 8) | b;
}

void init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(135 / 256.f, 206 / 256.f, 235 / 256.f, 1.0);
    Light::load("resources/light_gradient.raw", Point{338, 42});
    std::vector<GLfloat> verts;
    verts = {-640, 0, 640, 0, -640, -360, 640, -360}; // grass
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{0, 0}, 0x3f9b0bFF, 0.0f, Point{0, 0}));
    verts = {0, 0, 150, 0, 0, 120, 150, 120}; // house
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-150, 60}, 0xB97A57FF, 0.0f, Point{75, 60}));
    verts = {0, 0, 25, 0, 0, 50, 25, 50}; // chimney
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-100, 145}, 0x4b4b4bFF, 0.0f, Point{12.5, 0}));
    verts = {-15, 0, 165, 0, 75, 100}; // roof
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-150, 120}, 0xFF0000FF, 0.0f, Point{75, 0}));
    verts = {0, 0, 40, 0, 0, 70, 40, 70}; // door
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-190, 0}, 0x4b4b4bFF, 0.0f, Point{20, 0}));
    verts = {0, 0, 70, 0, 0, 50, 70, 50}; // window
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-125, 60}, 0x4b4b4bFF, 0.0f, Point{35, 25}));
    verts = {0, 0, 60, 0, 0, 40, 60, 40}; // light
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-125, 60}, 0x87CEEB, 0.0f, Point{30, 20}));
    verts = {0, 0, 60, 0, 0, 40, 60, 40}; // light
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-125, 60}, 0x87CEEB, 0.0f, Point{30, 20}, Point{1, 1}, false));
    verts = {0, -1, 70, -1, 0, 1, 70, 1}; // window frame hor
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-125, 60}, 0x4b4b4bFF, 0.0f, Point{35, 0}));
    verts = {-1, 0, 1, 0, -1, 50, 1, 50}; // window frame vert
    pols.push_back(std::make_shared<MyPolygon>(verts, Point{-125, 60}, 0x4b4b4bFF, 0.0f, Point{0, 25}));
    verts = {0, 0, 5, 0, 0, 5, 5, 5};

    for (size_t i = 0; i < 5; ++i)
    {
        unsigned int color = flowerColors[colorDist(gen)];
        for (size_t j = 0; j < 127; ++j)
        {
            int x = -630 + (10.f / 1) * j;
            int y = 100 * std::sin(j / 10.f) - 250 + 25 * i;
            pols.push_back(std::make_shared<MyPolygon>(verts, Point{x, y}, color, 45.0f, Point{2.5f, 2.5f}));
        }
    }
    bg.push_back(std::make_shared<Image>("resources/background_0.raw", Point{3072, 437}, Point{-122, 25}, 0, Point{1536, 217.5}, Point{1280.0f / 1024, 1}));
    bg.push_back(std::make_shared<Image>("resources/background_1.raw", Point{3072, 535}, Point{166, -25}, 0, Point{1536, 266.5}, Point{1280.0f / 1024, 1}));
    bg.push_back(std::make_shared<Image>("resources/background_2.raw", Point{3072, 535}, Point{-332, -75}, 0, Point{1536, 266.5}, Point{1280.0f / 1024, 1}));
    bg.push_back(std::make_shared<Image>("resources/background_3.raw", Point{3072, 535}, Point{444, -125}, 0, Point{1536, 266.5}, Point{1280.0f / 1024, 1}));
    sun = std::make_shared<Celestial>(1200, 0xffee50, Point{0, 300}, 90);
    moon = std::make_shared<Celestial>(1200, 0xF6F1D5, Point{0, 300}, 200);
}

void tick()
{
    glClear(GL_COLOR_BUFFER_BIT);

    sun->update();
    moon->update();
    sun->draw();
    moon->draw();

    Light::time = (145 - sun->getRotation()) / 180;
    unsigned int clearColor = mixColors(0x87CEEB, Light::getLightColor());
    glClearColor(((clearColor >> 16) & 0xFF) / 256.f, ((clearColor >> 8) & 0xFF) / 256.f, (clearColor & 0xFF) / 256.f, 1.0);
    float raveAlpha = Light::time >= 0.85 ? 1 - 13.33 * abs(Light::time - 0.925) : 0;
    pols[7]->color = getRainbow(10 * Light::time) + 255 * (Light::time > 0.9 || Light::time < 0.3);
    glEnable(GL_TEXTURE_2D);
    // for (const auto &img : bg)
    //     img->draw();
    bg[0]->draw();
    glDisable(GL_TEXTURE_2D);

    for (const auto &pol : pols)
        pol->draw();

    // bg[3]->pos.first += 0.1 * 0.5;
    // bg[2]->pos.first += 0.075 * 0.5;
    // bg[1]->pos.first += 0.05 * 0.5;
    // bg[0]->pos.first += 0.025 * 0.5;
    // for (const auto &img : bg)
    //     img->pos.first = img->pos.first >= 512 ? -512 : img->pos.first;

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL GLUT Starter");

    init();
    glutDisplayFunc(tick);
    glutMainLoop();
    return 0;
}
