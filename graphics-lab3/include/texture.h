#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include "stb_image.h"

class Texture {
public:
    static GLuint loadTexture(const std::string& name, const std::string& path);
    static GLuint loadCubemap(const std::string& name, const std::vector<std::string>& paths);
    static GLuint get(const std::string& name);
    static void unload();

private:
    static std::unordered_map<std::string, GLuint> textures;
};
