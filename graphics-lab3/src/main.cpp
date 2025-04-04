#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION


// Camera Variables
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float speed = 0.1f, sensitivity = 0.1f;

// Forward Vector
float frontX = 0.0f, frontY = 0.0f, frontZ = -1.0f;

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data) {
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void updateCameraDirection() {
    float radYaw = yaw * (M_PI / 180.0f);
    float radPitch = pitch * (M_PI / 180.0f);

    frontX = cos(radYaw) * cos(radPitch);
    frontY = sin(radPitch);
    frontZ = sin(radYaw) * cos(radPitch);

    // Normalize
    float len = sqrt(frontX * frontX + frontY * frontY + frontZ * frontZ);
    frontX /= len;
    frontY /= len;
    frontZ /= len;
}

// Keyboard Input
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = speed;
    if (key == 'w') { // Move forward
        cameraX += frontX * moveSpeed;
        cameraY += frontY * moveSpeed;
        cameraZ += frontZ * moveSpeed;
    }
    if (key == 's') { // Move backward
        cameraX -= frontX * moveSpeed;
        cameraY -= frontY * moveSpeed;
        cameraZ -= frontZ * moveSpeed;
    }
    if (key == 'd') { // Move right
        cameraX -= cos(yaw * (M_PI / 180.0f) - M_PI_2) * moveSpeed;
        cameraZ -= sin(yaw * (M_PI / 180.0f) - M_PI_2) * moveSpeed;
    }
    if (key == 'a') { // Move left
        cameraX += cos(yaw * (M_PI / 180.0f) - M_PI_2) * moveSpeed;
        cameraZ += sin(yaw * (M_PI / 180.0f) - M_PI_2) * moveSpeed;
    }
    glutPostRedisplay();
}

// Mouse Input
void mouseMotion(int x, int y) {
    static bool firstMouse = true;
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    if (firstMouse) {
        lastX = centerX;
        lastY = centerY;
        firstMouse = false;
    }

    float xOffset = (x - lastX) * sensitivity;
    float yOffset = (lastY - y) * sensitivity; // Reversed Y-coordinates

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraDirection();

    // Reset mouse to center
    glutWarpPointer(centerX, centerY);
    lastX = centerX;
    lastY = centerY;
    glutPostRedisplay();
}


// Render Scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 
              cameraX + frontX, cameraY + frontY, cameraZ + frontZ, 
              0.0f, 1.0f, 0.0f);

    glutWireTeapot(1.0);
    glutSwapBuffers();
}

// OpenGL Setup
void init() {
    glEnable(GL_DEPTH_TEST);
    glutSetCursor(GLUT_CURSOR_NONE);
}

// Reshape Window
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("FreeGLUT 3D Camera");
    glutFullScreen();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);

    init();
    glutMainLoop();
    return 0;
}
