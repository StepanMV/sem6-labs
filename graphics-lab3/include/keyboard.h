#pragma once

#include "transform.h"
#include <GLFW/glfw3.h>

class Keyboard
{
    friend int main();

private:
    static void processInput(GLFWwindow *window);
};
