#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "stb_image.h"

#include "cube.h"
#include "camera.h"
#include "keyboard.h"
#include "defines.h"

Cube *cube;
GLuint shaderProgram;

void reshape(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    cube->draw(shaderProgram, Camera::getView(), Camera::getProjection());
}

void myinit()
{
    cube = new Cube({"sky0", "sky1"});
    shaderProgram = cube->loadShaders("resources/shaders/texture.vert", "resources/shaders/texture.frag");
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "GLFW 3D Camera",
                                          FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    reshape(window, SCREEN_W, SCREEN_H);
    myinit();

    while (!glfwWindowShouldClose(window))
    {
        Keyboard::processInput(window);
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
