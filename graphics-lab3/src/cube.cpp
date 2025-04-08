#include "cube.h"

#include "texture.h"
#include "camera.h"

Cube::Cube(const std::vector<std::string> &textureNames)
{
    initGeometry();
    for (const auto &texName : textureNames)
    {
        textures.push_back(Texture::get(texName));
    }
    initShaders();
}

void Cube::initGeometry()
{
    GLfloat vertices[] = {
        // positions          // texture coordinates
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Front face
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Back face
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 1.0f,

        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // Top face
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom face
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,

        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Right face
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Left face
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,       // Front face
        4, 5, 6, 6, 7, 4,       // Back face
        8, 9, 10, 10, 11, 8,    // Top face
        12, 13, 14, 14, 15, 12, // Bottom face
        16, 17, 18, 18, 19, 16, // Right face
        20, 21, 22, 22, 23, 20  // Left face
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

GLuint Cube::loadShader(const char *filepath, GLenum shaderType)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    const char *sourceCStr = source.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCStr, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *log = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        std::cerr << "Shader compilation failed: " << log << std::endl;
        delete[] log;
    }

    return shader;
}

void Cube::initShaders()
{
    GLuint vertexShader = loadShader("resources/shaders/cube.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader("resources/shaders/cube.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *log = new GLchar[logLength];
        glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
        std::cerr << "Shader program linking failed: " << log << std::endl;
        delete[] log;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shader = shaderProgram;
}

void Cube::draw()
{
    glUseProgram(this->shader);

    GLint viewLoc = glGetUniformLocation(this->shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Camera::getView()));

    GLint projectionLoc = glGetUniformLocation(this->shader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Camera::getProjection()));

    for (GLuint i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        std::string textureName = "texture" + std::to_string(i + 1);
        GLuint textureLoc = glGetUniformLocation(this->shader, textureName.c_str());
        glUniform1i(textureLoc, i);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    GLint modelLoc = glGetUniformLocation(this->shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
