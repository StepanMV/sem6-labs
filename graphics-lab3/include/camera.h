#pragma once

#include "transform.h"
#include <GLFW/glfw3.h>

class Camera
{
    friend int main();

public:
    inline static const glm::vec3 &getFront() { return front; }
    static glm::mat4 getView();
    static glm::mat4 getProjection();

    static Transform transform;

private:
    static void updateCameraDirection();
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    static bool starting;
    static glm::vec2 prevMousePos;
    static glm::vec3 front;
};
