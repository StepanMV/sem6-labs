#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "transform.h"

class Cube
{
public:
    Cube(const std::string &textureName);

    void draw();
    
    Transform transform;

private:
    void initGeometry();
    void initShaders();
    GLuint loadShader(const char *filepath, GLenum shaderType);

    GLuint VAO, VBO, EBO;
    GLuint shader;
    GLuint cubemapTexture;
};
