#include "transform.h"

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::getMatrix() const
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
    glm::quat rotQuat = glm::quat(rotation);
    glm::mat4 rot = glm::toMat4(rotQuat);
    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
    return trans * rot * scl;
}

// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));