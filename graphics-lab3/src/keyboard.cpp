#include "keyboard.h"

#include "camera.h"
#include "defines.h"

void Keyboard::processInput(GLFWwindow *window)
{
    auto& cameraPos = Camera::transform.position;
    auto& cameraRot = Camera::transform.rotation;
    auto& cameraFront = Camera::getFront();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos.x += cameraFront.x * MOVE_SPEED;
        cameraPos.y += cameraFront.y * MOVE_SPEED;
        cameraPos.z += cameraFront.z * MOVE_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos.x -= cameraFront.x * MOVE_SPEED;
        cameraPos.y -= cameraFront.y * MOVE_SPEED;
        cameraPos.z -= cameraFront.z * MOVE_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos.x -= cos(cameraRot[0] * (M_PI / 180.0f) - M_PI_2) * MOVE_SPEED;
        cameraPos.z -= sin(cameraRot[0] * (M_PI / 180.0f) - M_PI_2) * MOVE_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos.x += cos(cameraRot[0] * (M_PI / 180.0f) - M_PI_2) * MOVE_SPEED;
        cameraPos.z += sin(cameraRot[0] * (M_PI / 180.0f) - M_PI_2) * MOVE_SPEED;
    }
}