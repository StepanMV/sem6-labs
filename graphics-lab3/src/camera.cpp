#include "camera.h"

#include "defines.h"

bool Camera::starting = true;
Transform Camera::transform;
glm::vec2 Camera::prevMousePos;
glm::vec3 Camera::front;

glm::mat4 Camera::getView()
{
    return glm::lookAt(
        glm::vec3(transform.position.x, transform.position.y, transform.position.z),                               // Camera position
        glm::vec3(transform.position.x + front.x, transform.position.y + front.y, transform.position.z + front.z), // Look-at point
        glm::vec3(0.0f, 1.0f, 0.0f)                                                                                // Up vector
    );
}

glm::mat4 Camera::getProjection()
{
    return glm::perspective(
        glm::radians(FOV),          // Field of view
        (float)SCREEN_W / SCREEN_H, // Aspect ratio
        0.1f, 100.0f                // Near and far planes
    );
}

void Camera::updateCameraDirection()
{
    float radYaw = transform.rotation[0] * (M_PI / 180.0f);
    float radPitch = transform.rotation[1] * (M_PI / 180.0f);

    front.x = cos(radYaw) * cos(radPitch);
    front.y = sin(radPitch);
    front.z = sin(radYaw) * cos(radPitch);

    float len = sqrt(front.x * front.x + front.y * front.y + front.z * front.z);
    front.x /= len;
    front.y /= len;
    front.z /= len;
}

void Camera::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (starting)
    {
        prevMousePos.x = xpos;
        prevMousePos.y = ypos;
        starting = false;
    }

    float xOffset = (xpos - prevMousePos.x) * SENS;
    float yOffset = (prevMousePos.y - ypos) * SENS;

    prevMousePos.x = xpos;
    prevMousePos.y = ypos;

    transform.rotation[0] += xOffset;
    transform.rotation[1] += yOffset;

    if (transform.rotation[1] > 89.0f)
        transform.rotation[1] = 89.0f;
    if (transform.rotation[1] < -89.0f)
        transform.rotation[1] = -89.0f;

    updateCameraDirection();
}